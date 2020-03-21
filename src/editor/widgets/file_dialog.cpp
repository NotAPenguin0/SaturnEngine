#include <editor/widgets/file_dialog.hpp>

#ifdef _WIN32
#    include <Windows.h>
#    include <shlobj_core.h>
#    include <shlwapi.h>
#    include <shobjidl.h>
#endif

namespace editor {

namespace impl {

// Indices of file types
#define INDEX_WORDDOC 1
#define INDEX_WEBPAGE 2
#define INDEX_TEXTDOC 3

// Controls
#define CONTROL_GROUP 2000
#define CONTROL_RADIOBUTTONLIST 2
#define CONTROL_RADIOBUTTON1 1
#define CONTROL_RADIOBUTTON2                                                   \
    2 // It is OK for this to have the same ID as CONTROL_RADIOBUTTONLIST,
      // because it is a child control under CONTROL_RADIOBUTTONLIST

// IDs for the Task Dialog Buttons
#define IDC_BASICFILEOPEN 100
#define IDC_ADDITEMSTOCUSTOMPLACES 101
#define IDC_ADDCUSTOMCONTROLS 102
#define IDC_SETDEFAULTVALUESFORPROPERTIES 103
#define IDC_WRITEPROPERTIESUSINGHANDLERS 104
#define IDC_WRITEPROPERTIESWITHOUTUSINGHANDLERS 105

#ifdef _WIN32
class CDialogEventHandler : public IFileDialogEvents,
                            public IFileDialogControlEvents {
public:
    // IUnknown methods
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) {
        static const QITAB qit[] = {
            QITABENT(CDialogEventHandler, IFileDialogEvents),
            QITABENT(CDialogEventHandler, IFileDialogControlEvents),
            {0},
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&_cRef); }

    IFACEMETHODIMP_(ULONG) Release() {
        long cRef = InterlockedDecrement(&_cRef);
        if (!cRef) delete this;
        return cRef;
    }

    // IFileDialogEvents methods
    IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
    IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP
    OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) {
        return S_OK;
    };
    IFACEMETHODIMP OnTypeChange(IFileDialog* pfd);
    IFACEMETHODIMP
    OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) {
        return S_OK;
    };

    // IFileDialogControlEvents methods
    IFACEMETHODIMP
    OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem);
    IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) {
        return S_OK;
    };
    IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) {
        return S_OK;
    };
    IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) {
        return S_OK;
    };

    CDialogEventHandler() : _cRef(1){};

private:
    virtual ~CDialogEventHandler(){};
    long _cRef;
};

// IFileDialogEvents methods
// This method gets called when the file-type is changed (combo-box selection
// changes). For sample sake, let's react to this event by changing the
// properties show.
HRESULT CDialogEventHandler::OnTypeChange(IFileDialog* pfd) {
    IFileSaveDialog* pfsd;
    HRESULT hr = pfd->QueryInterface(&pfsd);
    if (SUCCEEDED(hr)) {
        UINT uIndex;
        hr = pfsd->GetFileTypeIndex(&uIndex); // index of current file-type
        if (SUCCEEDED(hr)) {
            IPropertyDescriptionList* pdl = NULL;

            switch (uIndex) {
                case INDEX_WORDDOC:
                    // When .doc is selected, let's ask for some arbitrary
                    // property, say Title.
                    hr = PSGetPropertyDescriptionListFromString(
                        L"prop:System.Title", IID_PPV_ARGS(&pdl));
                    if (SUCCEEDED(hr)) {
                        // FALSE as second param == do not show default
                        // properties.
                        hr = pfsd->SetCollectedProperties(pdl, FALSE);
                        pdl->Release();
                    }
                    break;

                case INDEX_WEBPAGE:
                    // When .html is selected, let's ask for some other
                    // arbitrary property, say Keywords.
                    hr = PSGetPropertyDescriptionListFromString(
                        L"prop:System.Keywords", IID_PPV_ARGS(&pdl));
                    if (SUCCEEDED(hr)) {
                        // FALSE as second param == do not show default
                        // properties.
                        hr = pfsd->SetCollectedProperties(pdl, FALSE);
                        pdl->Release();
                    }
                    break;

                case INDEX_TEXTDOC:
                    // When .txt is selected, let's ask for some other arbitrary
                    // property, say Author.
                    hr = PSGetPropertyDescriptionListFromString(
                        L"prop:System.Author", IID_PPV_ARGS(&pdl));
                    if (SUCCEEDED(hr)) {
                        // TRUE as second param == show default properties as
                        // well, but show Author property first in list.
                        hr = pfsd->SetCollectedProperties(pdl, TRUE);
                        pdl->Release();
                    }
                    break;
            }
        }
        pfsd->Release();
    }
    return hr;
}

// IFileDialogControlEvents
// This method gets called when an dialog control item selection happens
// (radio-button selection. etc). For sample sake, let's react to this event by
// changing the dialog title.
HRESULT CDialogEventHandler::OnItemSelected(IFileDialogCustomize* pfdc,
                                            DWORD dwIDCtl,
                                            DWORD dwIDItem) {
    IFileDialog* pfd = NULL;
    HRESULT hr = pfdc->QueryInterface(&pfd);
    if (SUCCEEDED(hr)) {
        if (dwIDCtl == CONTROL_RADIOBUTTONLIST) {
            switch (dwIDItem) {
                case CONTROL_RADIOBUTTON1:
                    hr = pfd->SetTitle(L"Longhorn Dialog");
                    break;

                case CONTROL_RADIOBUTTON2:
                    hr = pfd->SetTitle(L"Vista Dialog");
                    break;
            }
        }
        pfd->Release();
    }
    return hr;
}

// Instance creation helper
HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv) {
    *ppv = NULL;
    CDialogEventHandler* pDialogEventHandler =
        new (std::nothrow) CDialogEventHandler();
    HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr)) {
        hr = pDialogEventHandler->QueryInterface(riid, ppv);
        pDialogEventHandler->Release();
    }
    return hr;
}

// https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box
// https://github.com/microsoft/Windows-classic-samples/blob/master/Samples/Win7Samples/winui/shell/appplatform/commonfiledialog/CommonFileDialogApp.cpp
static HRESULT win32_show_file_dialog(fs::path& path,
                                      fs::path const& dir,
                                      DWORD user_flags,
                                      stl::vector<FileType> const& filetypes) {
    IFileDialog* file_dialog = nullptr;
    // Create the window
    HRESULT hr =
        CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
                         IID_PPV_ARGS(&file_dialog));
    if (!SUCCEEDED(hr)) return hr;

    //	lpstrInitialDir

    // Create event handler object and hook it up
    IFileDialogEvents* file_dialog_events = nullptr;
    hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&file_dialog_events));
    if (!SUCCEEDED(hr)) return hr;

    DWORD cookie;
    hr = file_dialog->Advise(file_dialog_events, &cookie);
    if (!SUCCEEDED(hr)) return hr;

    // Set options
    DWORD flags;
    hr = file_dialog->GetOptions(&flags);
    if (!SUCCEEDED(hr)) return hr;

    hr = file_dialog->SetOptions(flags | user_flags);
    if (!SUCCEEDED(hr)) return hr;

    // Set folder stuff. Temporary for testing
    PIDLIST_ABSOLUTE pidl;
    hr = ::SHParseDisplayName(dir.wstring().c_str(), 0, &pidl, SFGAO_FOLDER, 0);
    if (!SUCCEEDED(hr)) return hr;
    IShellItem* psi;
    hr = ::SHCreateShellItem(nullptr, nullptr, pidl, &psi);
    if (!SUCCEEDED(hr)) {
        ILFree(pidl);
        return hr;
    }
    file_dialog->SetFolder(psi);
    ILFree(pidl);

    // Set file types if needed
    if (!filetypes.empty()) {
        hr = file_dialog->SetFileTypes(
            filetypes.size(),
            // Wonky reinterpret_cast, watch this man. #Danger
            reinterpret_cast<COMDLG_FILTERSPEC const*>(filetypes.data()));
        if (!SUCCEEDED(hr)) return hr;
        // Default to first file type
        hr = file_dialog->SetFileTypeIndex(0);
        if (!SUCCEEDED(hr)) return hr;
    }

    hr = file_dialog->Show(nullptr);
    if (!SUCCEEDED(hr)) return hr;

    // Get the result
    IShellItem* result = nullptr;
    hr = file_dialog->GetResult(&result);
    if (!SUCCEEDED(hr)) return hr;

    PWSTR file_path = nullptr;
    hr = result->GetDisplayName(SIGDN_FILESYSPATH, &file_path);
    if (!SUCCEEDED(hr)) return hr;

    path = file_path;
    CoTaskMemFree(file_path);
    result->Release();
    file_dialog->Unadvise(cookie);
    file_dialog_events->Release();
    file_dialog->Release();

    return hr;
}

#endif
} // namespace impl

bool FileDialog::show(Flags flags, fs::path dir, stl::vector<FileType> filetypes) {
#ifdef _WIN32
    DWORD user_flags = 0;
    if (flags & Flags::PickFiles) { user_flags |= FOS_FORCEFILESYSTEM; }
    if (flags & Flags::PickFolders) { user_flags |= FOS_PICKFOLDERS; }

    HRESULT hresult = impl::win32_show_file_dialog(result, dir, user_flags, filetypes);
    return SUCCEEDED(hresult);
#endif
}

fs::path FileDialog::get_result() const { return result; }

} // namespace editor
