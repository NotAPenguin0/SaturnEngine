#ifndef SATURN_SELECT_FILE_DIALOG_HPP_
#define SATURN_SELECT_FILE_DIALOG_HPP_

#include <filesystem>
#include <stl/vector.hpp>
#include <string>

namespace fs = std::filesystem;

namespace editor {

struct FileType {
    const wchar_t* name;
    const wchar_t* filter;
};

class FileDialog {
public:
    enum Flags {
		PickFiles = 1,
		PickFolders = 2
    };

    bool show(Flags flags, fs::path dir, stl::vector<FileType> filetypes = {});
    fs::path get_result() const;

private:
    fs::path result;
};

} // namespace Saturn::Editor

#endif