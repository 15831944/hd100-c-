#define VER_Major		1
#define VER_Minor		0
#define VER_Release	2058
#define VER_Build		1595
#define VER_Version	MAKELONG(MAKEWORD(VER_Major, VER_Minor), VER_Release)

#define _Stringizing(v)	#v
#define _VerJoin(a, b, c, d)  _Stringizing(a.b.c.d)

#define STR_BuildDate		TEXT(__DATE__)
#define STR_BuildTime		TEXT(__TIME__)
#define STR_BuilDateTime		TEXT(__DATE__) TEXT(" ") TEXT(__TIME__)
#define STR_Version		TEXT(_VerJoin(VER_Major, VER_Minor, VER_Release, VER_Build))

#define STR_AppName		TEXT("HD100  ")
#define STR_Author		TEXT("yqq")
#define STR_Corporation		TEXT("kaifa")
#define STR_Web			TEXT("www.")
#define STR_Email			TEXT("qingquanyin@kaifa.com")
#define STR_WebUrl		TEXT("http://") STR_Web
#define STR_EmailUrl		TEXT("mailto:") STR_Email TEXT("Subject=") STR_AppName

#ifdef _CHS
#define STR_Description		TEXT("HD100 Application")
#define STR_Copyright		TEXT("Copyright (C) 2018 ") STR_Corporation TEXT("，保留所有权利。")
#else // _CHS
#define STR_Description		TEXT("HD100 Application")
#define STR_Copyright		TEXT("Copyright (C) 2018 ") STR_Corporation TEXT(". All rights reserved.")
#endif // _CHS
