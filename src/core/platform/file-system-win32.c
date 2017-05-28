
#include <Windows.h>

#include "char-buf.h"
#include "file-system.h"
#include "mem.h"

static int32_t get_file_count(char *search);

bool create_path(char_buf *path)
{
    return true;
}

bool get_path_to_executable(char_buf *path)
{
    DWORD result = GetModuleFileName(NULL, path->buf, path->size);

    if (GetLastError() != 0)
        return false;

    //strip the executable name by shifting null terminator
    for (int32_t i = result; i >= 0; i--) {
        if (path->buf[i] == '\\') {
            path->buf[i] = '\0';
            break;
        }
    }

    return true;
}

bool get_working_directory(char_buf *path)
{
    GetCurrentDirectory(path->size, path->buf);

    return GetLastError() == 0;
}

bool set_working_directory(char_buf *path)
{
    return SetCurrentDirectory(path->buf);
}

bool open_directory(directory_stats *dir, char_buf *path)
{
    char search[path->size + 2];
    write_char_buf_format(search, path->size + 2, path->size + 1, "%s\\*", path->buf);

    int32_t num_files = get_file_count(search);

    if (num_files < 0) {
        return false;
    }

    if (num_files == 0) {
        return true;
    }

    WIN32_FIND_DATA find_data;
    HANDLE handle = FindFirstFileA(search, &find_data);

    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    size_t file_stats_sz = sizeof(file_stats) * num_files;
    file_stats *files = MALLOC_HEAP(file_stats_sz, 0, "file stats");

    int32_t i = 0;

    do {
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            size_t file_dest_sz = MAX_FILE_NAME + 1;
            size_t file_count = MAX_FILE_NAME;
            write_char_buf_count(files[i].name, file_dest_sz, find_data.cFileName, file_count);

            size_t high_sz = (size_t)find_data.nFileSizeLow;
            size_t low_sz = (size_t)find_data.nFileSizeHigh;
            files[i].size = high_sz * ((size_t)MAXDWORD + 1) + low_sz;
            files[i].extension[0] = 0;

            char *period = files[i].name;
            char c = 0;

            //file extension is everything after the first period
            while ((c = (*period++))) {
                if (c == '.') {
                    size_t ext_dest_sz = MAX_FILE_EXTENSION + 1;
                    size_t ext_count = MAX_FILE_EXTENSION;
                    write_char_buf_count(files[i].extension, ext_dest_sz, period, ext_count);
                    break;
                }
            }

            i++;
        }
    } while (i <= num_files && FindNextFileA(handle, &find_data) != 0);

    FindClose(handle);

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        return false;
    }

    dir->files = files;
    dir->num_files = num_files;

    return true;
}

void close_directory(directory_stats *dir)
{
    if (dir->files) {
        MEM_FREE_HEAP(dir->files);
    }
}

static int32_t get_file_count(char *search)
{
    WIN32_FIND_DATA file;
    HANDLE handle = FindFirstFileA(search, &file);

    if (handle == INVALID_HANDLE_VALUE) {
        return -1;
    }

    int32_t count = 0;

    do {
        if (!(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            count++;
        }

    } while (FindNextFileA(handle, &file) != 0);

    FindClose(handle);

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        return -1;
    }

    return count;
}
