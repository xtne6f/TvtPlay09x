﻿#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#define ARRAY_SIZE _countof

#define WM_ASFLT_STRETCH    (WM_APP + 1)
LRESULT ASFilterSendMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL ASFilterPostMessage(UINT Msg, WPARAM wParam, LPARAM lParam);

#define ICON_SIZE 16

bool ComposeMonoColorIcon(HDC hdcDest, int destX, int destY, HBITMAP hbm, LPCTSTR pIdxList);
int CompareTStrI(const void *str1, const void *str2);
int CompareTStrIX(const void *str1, const void *str2);
BOOL WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int value, LPCTSTR lpFileName);
LONGLONG CalcHash(const LPBYTE pbData, DWORD dwDataLen, DWORD dwSalt);

typedef struct {
	int           sync;
	int           transport_error_indicator;
	int           payload_unit_start_indicator;
	int           transport_priority;
	int           pid;
	int           transport_scrambling_control;
	int           adaptation_field_control;
	int           continuity_counter;
} TS_HEADER;

typedef struct {
	int           adaptation_field_length;
	int           discontinuity_counter;
	int           random_access_indicator;
	int           elementary_stream_priority_indicator;
	int           pcr_flag;
	int           opcr_flag;
	int           splicing_point_flag;
	int           transport_private_data_flag;
	int           adaptation_field_extension_flag;
	unsigned int  pcr_45khz;
} ADAPTATION_FIELD; // (partial)

int select_unit_size(unsigned char *head, unsigned char *tail);
unsigned char *resync(unsigned char *head, unsigned char *tail, int unit_size);
void extract_ts_header(TS_HEADER *dst, const unsigned char *packet);
void extract_adaptation_field(ADAPTATION_FIELD *dst, const unsigned char *data);

#define ABSTRACT_DECL			__declspec(novtable)
#define ABSTRACT_CLASS(name)	ABSTRACT_DECL name abstract

#define APP_NAME TEXT("TvtPlay")

namespace StdUtil {
int vsnprintf(wchar_t *s,size_t n,const wchar_t *format,va_list args);
inline size_t strnlen(const wchar_t *s,size_t n) { return ::wcsnlen(s,n); }
}

/////////////////////////////////////////////////////////////////////////////
// トレースクラス
/////////////////////////////////////////////////////////////////////////////

class CTracer
{
	TCHAR m_szBuffer[256];
public:
	virtual ~CTracer() {}
	void Trace(LPCTSTR pszOutput, ...);
	void TraceV(LPCTSTR pszOutput,va_list Args);
protected:
	virtual void OnTrace(LPCTSTR pszOutput)=0;
};

__declspec(restrict) LPWSTR DuplicateString(LPCWSTR pszString);
bool ReplaceString(LPWSTR *ppszString,LPCWSTR pszNewString);

inline bool IsStringEmpty(LPCWSTR pszString) {
	return pszString==NULL || pszString[0]==L'\0';
}
inline LPCWSTR NullToEmptyString(LPCWSTR pszString) {
	return pszString!=NULL?pszString:L"";
}

COLORREF MixColor(COLORREF Color1,COLORREF Color2,BYTE Ratio=128);
bool CompareLogFont(const LOGFONT *pFont1,const LOGFONT *pFont2);

class CDynamicString {
protected:
	LPTSTR m_pszString;
public:
	CDynamicString();
	CDynamicString(const CDynamicString &String);
#ifdef MOVE_SEMANTICS_SUPPORTED
	CDynamicString(CDynamicString &&String);
#endif
	explicit CDynamicString(LPCTSTR pszString);
	virtual ~CDynamicString();
	CDynamicString &operator=(const CDynamicString &String);
#ifdef MOVE_SEMANTICS_SUPPORTED
	CDynamicString &operator=(CDynamicString &&String);
#endif
	CDynamicString &operator+=(const CDynamicString &String);
	CDynamicString &operator=(LPCTSTR pszString);
	CDynamicString &operator+=(LPCTSTR pszString);
	bool operator==(const CDynamicString &String) const;
	bool operator!=(const CDynamicString &String) const;
	LPCTSTR Get() const { return m_pszString; }
	LPCTSTR GetSafe() const { return NullToEmptyString(m_pszString); }
	bool Set(LPCTSTR pszString);
	bool Set(LPCTSTR pszString,size_t Length);
	bool Attach(LPTSTR pszString);
	int Length() const;
	void Clear();
	bool IsEmpty() const;
	int Compare(LPCTSTR pszString) const;
	int CompareIgnoreCase(LPCTSTR pszString) const;
};

class CGlobalLock
{
	HANDLE m_hMutex;
	bool m_fOwner;

	// delete
	CGlobalLock(const CGlobalLock &);
	CGlobalLock &operator=(const CGlobalLock &);

public:
	CGlobalLock();
	~CGlobalLock();
	bool Create(LPCTSTR pszName);
	bool Wait(DWORD Timeout=INFINITE);
	void Close();
	void Release();
};

#endif // INCLUDE_UTIL_H
