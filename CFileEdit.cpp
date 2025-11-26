#include "pch.h"
#include "CFileEdit.h"



IMPLEMENT_DYNAMIC(CFileEdit,CEdit)

CFileEdit::CFileEdit() {

}
CFileEdit::~CFileEdit() {

}

BEGIN_MESSAGE_MAP(CFileEdit,CEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


void CFileEdit::OnLButtonDblClk(UINT nFlags, CPoint point) {
	CString filter = L"INI 文件 (*.ini)|*.ini|所有文件 (*.*)|*.*||";

	CFileDialog dlg(
		TRUE,		//TRUE:打开文件
		L"ini",		//默认拓展
		NULL,
		OFN_FILEMUSTEXIST, //必须存在的文件
		filter,
		this
	);

	CString curpath = GetExePath();
	curpath += L"\\DATA\\INI";
	MakeDirs(std::wstring(curpath));

	dlg.m_ofn.lpstrInitialDir = curpath;

	CString filePath;
	if (dlg.DoModal() == IDOK) {
		filePath = dlg.GetPathName();
		SetWindowText(filePath);// 直接设置到编辑框
	}

	////从ini中获取onnx文件加载到控件
	//WCHAR buf[256] = { 0 };
	//bool ret = GetPrivateProfileString(L"ONNXFile", L"OnnxFile", L"", buf, 256, filePath);
	//if(ret && !m_parentDlg) {
	//	//m_parentDlg->SetModlePath(buf);
	//}

	CEdit::OnLButtonDblClk(nFlags, point);
}



CString CFileEdit::GetExePath()
{
	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);

	CString exePath = path;
	int pos = exePath.ReverseFind('\\');
	if (pos != -1)
		exePath = exePath.Left(pos);

	return exePath;   // 返回: 例如 D:\MyApp\bin\Release
}


bool CFileEdit::MakeDirs(const std::wstring& path)
{
	if (path.empty()) return false;

	std::wstring tmp = path;
	if (tmp.back() != L'\\')
		tmp += L'\\';

	size_t pos = 0;

	while ((pos = tmp.find(L'\\', pos + 1)) != std::wstring::npos)
	{
		std::wstring sub = tmp.substr(0, pos);

		// 跳过 “C:” 或 “D:” 这种盘符
		if (sub.length() > 2)
		{
			DWORD attr = GetFileAttributesW(sub.c_str());

			if (attr == INVALID_FILE_ATTRIBUTES)
			{
				// 目录不存在，尝试创建
				if (!CreateDirectoryW(sub.c_str(), NULL))
				{
					DWORD err = GetLastError();

					// 允许：目录已经存在
					if (err != ERROR_ALREADY_EXISTS)
					{
						// 创建失败
						return false;
					}
				}
			}
		}
	}

	return true;
}