#ifndef SATURN_SELECT_FILE_DIALOG_HPP_
#define SATURN_SELECT_FILE_DIALOG_HPP_

#include <filesystem>

namespace fs = std::filesystem;

namespace Saturn::Editor {

class SelectFileDialog {
public:
    enum Flags {
		PickFiles = 1,
		PickFolders = 2
    };

    bool show(Flags flags);
    fs::path get_result() const;

private:
    fs::path result;
};

} // namespace Saturn::Editor

#endif