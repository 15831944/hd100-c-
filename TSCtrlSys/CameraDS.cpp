#include "StdAfx.h"
#include "CameraDS.h"

//////////////////////////////////////////////////////////////////////////
void CCameraDS::XferCallback(SapXferCallbackInfo *pInfo)
{
	CCameraDS *pCamera= (CCameraDS *) pInfo->GetContext();

	// If grabbing in trash buffer, do not display the image, update the
	// appropriate number of frames on the status bar instead
	if (pInfo->IsTrash())
	{
		CString str;
		str.Format(_T("Frames acquired in trash buffer: %d"), pInfo->GetEventCount());
	//	pCamera->m_statusWnd.SetWindowText(str);
	}	
	else	// Refresh view
	{
		pCamera->m_Buffers->ReadRect(0, 0, 0, IMG_WIDTH, IMG_HEIGHT, pCamera->m_pBuffer);
		// Redraw the image.
		::PostMessage( pCamera->m_hWndShowImage, WM_REDRAWIMAGEFROMCCD , NULL, NULL );
//		pCamera->m_View->Show();
	}
}

CCameraDS::CCameraDS(void)
{
	m_bCameraInitFlag = false;
	m_pBuffer = NULL;

	m_AcqDevice = NULL;
	m_Buffers = NULL;	
	m_Xfer = NULL;
	m_View = NULL;

	m_strFilePath = "C:\\Program Files (x86)\\Teledyne DALSA\\Sapera\\CamFiles\\User\\T_Nano-M1280_Default_Default.ccf";
	m_serverName = "Nano-M1280"; 
	m_resourceIndex = 0;
}


CCameraDS::~CCameraDS(void)
{
	//////////////////////////////////////////////////////////////////////////
	DestroyObjects();
	if (m_View)			delete m_View;
	if (m_Xfer)			delete m_Xfer;
	if (m_Buffers)		delete m_Buffers;
	if (m_AcqDevice)	delete m_AcqDevice;
}

BOOL CCameraDS::DestroyObjects()
{
	// Destroy transfer object
	if (m_Xfer && *m_Xfer) m_Xfer->Destroy();

	// Destroy view object
	if (m_View && *m_View) m_View->Destroy();

	// Destroy buffer object
	if (m_Buffers && *m_Buffers) m_Buffers->Destroy();

	// Destroy acquisition object
	if (m_AcqDevice && *m_AcqDevice) m_AcqDevice->Destroy();

	return TRUE;
}

BOOL CCameraDS::CreateObjects()
{
	CWaitCursor wait;

	// Create acquisition object
	if (m_AcqDevice && !*m_AcqDevice && !m_AcqDevice->Create())
	{
		DestroyObjects();
		return FALSE;
	}

	// Create buffer object
	if (m_Buffers && !*m_Buffers)
	{
		if( !m_Buffers->Create())
		{
			DestroyObjects();
			return FALSE;
		}
		// Clear all buffers
		m_Buffers->Clear();
	}

	// Create view object
	if (m_View && !*m_View && !m_View->Create())
	{
		DestroyObjects();
		return FALSE;
	}

	// Set next empty with trash cycle mode for transfer
	if (m_Xfer && m_Xfer->GetPair(0))
	{
		if (!m_Xfer->GetPair(0)->SetCycleMode(SapXferPair::CycleNextWithTrash))
		{
			DestroyObjects();
			return FALSE;
		}
	}

	// Create transfer object
	if (m_Xfer && !*m_Xfer && !m_Xfer->Create())
	{
		DestroyObjects();
		return FALSE;
	}

	return TRUE;
}

BOOL CCameraDS::OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress)
{
	if (m_bCameraInitFlag)	return TRUE;

	m_hWndShowImage = hWndDisplay;
	if (NULL != m_pBuffer)
	{
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}

	m_pBuffer = new BYTE[IMG_WIDTH*IMG_HEIGHT];
	if (NULL == m_pBuffer)
	{
		AfxMessageBox("分配相机内存出错!");
		return FALSE;
	}

	lpDesIMGAddress = m_pBuffer;
	return Init();
}

BOOL CCameraDS::Init()
{
	{
		int nServerCount = SapManager::GetServerCount();
		for (int i = 0; i < nServerCount; i++)
		{
			// Does this server support "Acq" (frame-grabber) or "AcqDevice" (camera)?
// 			bool bAcq = (m_serverCategory == ServerAcq || m_serverCategory == ServerAll)
// 				&& (GetResourceCount(i, ResourceAcq) > 0);

			// when m_serverCategory is ServerAcqDevice, show only servers that have only one resource that is an acqdevice and no acq
			bool bAcqDevice = (SapManager::GetResourceCount(i,SapManager::ResourceAcqDevice) == 1);
			if (bAcqDevice)
			{
				char name[128];
				if (SapManager::GetServerName(i, name, sizeof(name)))
				{
					m_serverName = name;
				}
			}
		}
	}
	if (SapManager::GetServerIndex(CStringA(m_serverName)) != SapLocation::ServerUnknown)
	{
		// Add "AcqDevice" resources (cameras) to combo only if there are no frame grabber resources
		if (SapManager::GetResourceCount(CStringA(m_serverName), SapManager::ResourceAcq) == 0)
		{
			for (int i = 0; i < SapManager::GetResourceCount(CStringA(m_serverName), SapManager::ResourceAcqDevice); i++)
			{
				char name[128];
				SapManager::GetResourceName(CStringA(m_serverName), SapManager::ResourceAcqDevice, i, name, sizeof(name));
				//	m_cbResource.AddString(CString(name));
			}
		}
	}

	// Define objects
	m_AcqDevice	= new SapAcqDevice(SapLocation(CStringA(m_serverName), m_resourceIndex), m_strFilePath);
	m_Buffers	= new SapBufferWithTrash(2, m_AcqDevice);
	m_Xfer		= new SapAcqDeviceToBuf(m_AcqDevice, m_Buffers, XferCallback, this);
//	m_View      = new SapView(m_Buffers, SapHwndAutomatic);

	// 	// Attach sapview to image viewer
	// 	m_ImageWnd.AttachSapView(m_View);

	// Create all objects
	if (!CreateObjects()) {	return false; }
	m_Xfer->Grab();

	//m_ImageWnd.AttachEventHandler(this);
	// 	m_ImageWnd.CenterImage();
	// 	m_ImageWnd.Reset();
	m_bCameraInitFlag = true;
	return true;
}

BOOL CCameraDS::Init0()
{
	{
		UINT32   acqDeviceNumber;
		char*    acqServerName = new char[CORSERVER_MAX_STRLEN];
		//////// Ask questions to user to select acquisition board/device and config file ////////

		// Get total number of boards in the system
//		int serverIndex = 0;
		int serverCount = SapManager::GetServerCount();
		if (serverCount == 0)
		{
			AfxMessageBox("No device found!\n");
			return FALSE;
		}

// 		int devicesToSkip = 0;
// 		for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
// 		{
// 			if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
// 			{
// 				char serverName[CORSERVER_MAX_STRLEN];
// 				SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
// 				if (strstr(serverName,"CameraLink_") > 0)
// 					devicesToSkip++;
// 			}
// 		}

		// Scan the boards to find those that support acquisition
		BOOL serverFound = FALSE;
		BOOL cameraFound = FALSE;

		for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
		{
			if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq) != 0)
			{
				char serverName[CORSERVER_MAX_STRLEN];
				SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
				printf("%d: %s\n", serverIndex, serverName);
				serverFound = TRUE;
			}
			else if (SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
			{
				//char serverName[CORSERVER_MAX_STRLEN];
				SapManager::GetServerName(serverIndex, acqServerName, sizeof(acqServerName));
				if (strstr(acqServerName,"CameraLink_") > 0)
					continue;
				printf("%d: %s\n", serverIndex, acqServerName);
				cameraFound = TRUE;
			}
		}

		// At least one acquisition server must be available
		if (!serverFound && !cameraFound)
		{
			AfxMessageBox("No acquisition server found!\n");
			return FALSE;
		}

		//char key = (char)_getch();
		if (TRUE)//(key != 0)
		{
			// 			if (key == 'q')
			// 				return FALSE;

			int serverNum = 1; // char-to-int conversion
			if ((serverNum >= 1) && (serverNum < serverCount))
			{
				// update board name
				SapManager::GetServerName(serverNum, acqServerName, CORSERVER_MAX_STRLEN);
			}
			else
			{
				AfxMessageBox("Invalid selection!\n");
				return FALSE;
			}
		}
		else
		{
			AfxMessageBox("Invalid selection!\n");
			return FALSE;
		}


		// Scan all the acquisition devices on that server and show menu to user
		int deviceCount = SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcq);
		int cameraCount = (deviceCount == 0) ? SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcqDevice) : 0;

		// 		for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
		// 		{
		// 			char deviceName[CORPRM_GETSIZE(CORACQ_PRM_LABEL)];
		// 			SapManager::GetResourceName(acqServerName, SapManager::ResourceAcq, deviceIndex, deviceName, sizeof(deviceName));
		// 			printf("%d: %s\n", deviceIndex+1, deviceName);
		// 		}

		if (deviceCount == 0)
		{
			for (int cameraIndex = 0; cameraIndex < cameraCount; cameraIndex++)
			{
				char cameraName[CORPRM_GETSIZE(CORACQ_PRM_LABEL)];
				SapManager::GetResourceName(acqServerName, SapManager::ResourceAcqDevice, cameraIndex, cameraName, sizeof(cameraName));
				printf("%d: %s\n", cameraIndex+1, cameraName);
			}
		}


		//key = (char)_getch();
		if (TRUE)//(key != 0)
		{
			// 			if (key == 'q')
			// 				return FALSE;
			int deviceNum = 1;//key - '0'; // char-to-int conversion
			if ((deviceNum >= 1) && (deviceNum <= deviceCount+cameraCount))
			{				
				acqDeviceNumber = deviceNum-1;
			}
			else
			{
				AfxMessageBox("Invalid selection!\n");
				return FALSE;
			}
		}
		else
		{
			AfxMessageBox("Invalid selection!\n");
			return FALSE;
		}

		// 		// List all files in the config directory
		// 		char configPath[MAX_PATH];
		// 		configPath[0] = '\0';
		// 		GetEnvironmentVariable("SAPERADIR", configPath, sizeof(configPath));
		// 		configPath[sizeof(configPath) - 1] = '\0';
		// 		CorStrncat(configPath, "\\CamFiles\\User\\", sizeof(configPath));
		// 
		// 		char findPath[MAX_PATH];
		// 		CorStrncpy(findPath, configPath, MAX_PATH);
		// 		CorStrncat(findPath, "*.ccf", sizeof(findPath));
		// 
		// 		HANDLE fhandle;
		// 		WIN32_FIND_DATA fdata;
		// 		if ((fhandle = FindFirstFile(findPath, &fdata)) == INVALID_HANDLE_VALUE)
		// 		{
		// 			if (cameraCount==0)
		// 			{
		// 				printf("No config file found.\nUse CamExpert to generate a config file before running this example.\n");
		// 				return FALSE;
		// 			}
		// 		}
		// 
		// 		fhandle = FindFirstFile(findPath, &fdata);
		// 
		// 		// Try to find the last letter to choose
		// 
		// 		int configFileMenuCount = 0;
		// 		char lastCharMenu='x';
		// 		do
		// 		{
		// 			// Use numbers 0 to 9, then lowercase letters if there are more than 10 files
		// 			int configFileMenuShow = configFileMenuCount+1;
		// 			if (configFileMenuCount > 9)
		// 				lastCharMenu = (char)(configFileMenuShow - 10 + 'a');
		// 			configFileMenuCount++;
		// 		}
		// 		while (FindNextFile(fhandle, &fdata) && configFileMenuCount < MAX_CONFIG_FILES);
		// 		FindClose(fhandle);
		// 
		// 		printf("\nSelect the config file (or 'q' to quit)");
		// 		printf("\n.......................................\n");
		// 		if (deviceCount == 0 && cameraCount != 0)
		// 		{
		// 			printf("0: No config File.\n");
		// 		}
		// 
		// 		fhandle = FindFirstFile(findPath, &fdata);							//find first file
		// 		BOOL moreFilesAvailable = TRUE;
		// 
		// 		int configFileCount = 1;
		// 		while ((fhandle != INVALID_HANDLE_VALUE && moreFilesAvailable) && configFileCount < MAX_CONFIG_FILES)
		// 		{
		// 			// Use numbers 0 to 9, then lowercase letters if there are more than 10 files
		// 			if (configFileCount <= 9)
		// 				printf("%d: %s\n", configFileCount, fdata.cFileName);
		// 			else
		// 				printf("%c: %s\n", configFileCount - 10 + 'a', fdata.cFileName);
		// 			CorStrncpy(configFileNames[configFileCount], fdata.cFileName, sizeof(configFileNames[configFileCount]));
		// 			configFileCount++;
		// 			moreFilesAvailable = FindNextFile(fhandle, &fdata);
		// 		}
		// 
		// 		FindClose(fhandle);
		// 
		// 		key = (char)_getch();
		// 		if (key != 0)
		// 		{
		// 			if (key == '0' && cameraCount != 0)
		// 			{
		// 				CorStrncpy(configFileName, "NoFile",7); 
		// 				return TRUE;
		// 			}
		// 
		// 			if (key == 'q')
		// 				return FALSE;
		// 			// Use numbers 1 to 9, then lowercase letters if there are more than 9 files
		// 			int configNum;
		// 			if (key >= '0' && key <= '9')
		// 				configNum = key - '0'; // char-to-int conversion
		// 			else
		// 				configNum = key - 'a' + 10; // char-to-int conversion
		// 
		// 			if ((configNum >= 0) && (configNum <= configFileCount-1))
		// 			{
		// 				CorStrncpy(configFileName, configPath, sizeof(configPath));
		// 				CorStrncat(configFileName, configFileNames[configNum], sizeof(configFileNames[configNum]));
		// 			}
		// 			else
		// 			{
		// 				printf("Invalid selection!\n"); 
		// 				return FALSE;
		// 			}
		// 		}
		// 		else
		// 		{
		// 			printf("Invalid selection!\n");
		// 			return FALSE;
		// 		}
		// 
		// 		printf("\n");
		// 		return TRUE;

		SapAcquisition Acq;
		SapAcqDevice AcqDevice;
		SapBufferWithTrash Buffers;
		SapTransfer AcqToBuf = SapAcqToBuf(&Acq, &Buffers);
		SapTransfer AcqDeviceToBuf = SapAcqDeviceToBuf(&AcqDevice, &Buffers);
		SapTransfer* Xfer = NULL;
		SapView View;

		SapLocation loc(acqServerName, acqDeviceNumber);

		if (SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcq) > 0)
		{
			Acq = SapAcquisition(loc, m_strFilePath);
			Buffers = SapBufferWithTrash(2, &Acq);
			View = SapView(&Buffers, SapHwndAutomatic);
			AcqToBuf = SapAcqToBuf(&Acq, &Buffers, XferCallback, &View);
			Xfer = &AcqToBuf;

			// Create acquisition object
			if (!Acq.Create())
				goto FreeHandles;

		}

		else if (SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcqDevice) > 0)
		{
			if (strcmp(m_strFilePath, "NoFile") == 0)
				AcqDevice = SapAcqDevice(loc, FALSE);
			else
				AcqDevice = SapAcqDevice(loc, m_strFilePath);

			Buffers = SapBufferWithTrash(2, &AcqDevice);
			View = SapView(&Buffers, SapHwndAutomatic);
			AcqDeviceToBuf = SapAcqDeviceToBuf(&AcqDevice, &Buffers, XferCallback, &View);
			Xfer = &AcqDeviceToBuf;

			// Create acquisition object
			if (!AcqDevice.Create())
				goto FreeHandles;

		}

		// Create buffer object
		if (!Buffers.Create())
			goto FreeHandles;

		// Create transfer object
		if (Xfer && !Xfer->Create())
			goto FreeHandles;

		// Create view object
		if (!View.Create())
			goto FreeHandles;

		// Start continous grab
		Xfer->Grab();
//		return TRUE;

		// Stop grab
		Xfer->Freeze();
		if (!Xfer->Wait(5000))
			printf("Grab could not stop properly.\n");

FreeHandles:
		//unregister the acquisition callback
		Acq.UnregisterCallback();

		// Destroy view object
		if (!View.Destroy()) return FALSE;

		// Destroy transfer object
		if (Xfer && *Xfer && !Xfer->Destroy()) return FALSE;

		// Destroy buffer object
		if (!Buffers.Destroy()) return FALSE;

		// Destroy acquisition object
		if (!Acq.Destroy()) return FALSE;

		// Destroy acquisition object
		if (!AcqDevice.Destroy()) return FALSE;

		return 0;
	}
}	

// 设置为Trigger模式  bTriggerMode  true:设置为Trigger模式   false:连续采集模式
BOOL CCameraDS::SetTriggerMode(bool bTriggerMode)	
{
	return TRUE;
}

// 发送软触发
BOOL CCameraDS::SoftTrigger()	
{
	return TRUE;
}

// 等待callback方法获取一帧图像,图像数据保存在buf中
BOOL CCameraDS::WaitGrabOneFrame()
{
	CFunction::DelaySec(0.2);
	return TRUE;
}