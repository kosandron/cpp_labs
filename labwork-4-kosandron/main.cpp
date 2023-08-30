#include "parser/parser.h"
#include "archivator/archivator.h"

int main(int argc, const char* argv[]) {
    Options params = ReadOptions(argc, argv);
    if (params.create_archive) {
        createArchive(params.archive_name, params.files, params.control_bites_count);
    } else if (params.list_of_files) {
        printList(params.archive_name);
    } else if (params.extract_files) {
        std::cout << "Extract01\n";
        extractArchive(params.archive_name, params.files);
    } else if (params.append_file) {
        addFile(params.archive_name, params.files[0], params.control_bites_count);
    } else if (params.delete_file) {
        deleteFile(params.archive_name, params.files[0]);
    } else if (params.concatenate_archives) {
        mergeArchives(params.files[0], params.files[1], params.archive_name);
    }

    return 0;
}
