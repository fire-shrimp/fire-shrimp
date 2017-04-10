
#include <Windows.h>

#include "file-system.h"
#include "mem.h"
#include "char-buf.h"

static int32_t get_file_count(char *search);

bool create_path(struct char_buf *path)
{
	return true;
}

bool get_path_to_executable(struct char_buf *path)
{
	DWORD result = GetModuleFileName(NULL, path->buf, path->size);

	if(GetLastError() != 0)
		return false;

	//strip the executable name by shifting null terminator
	for(int32_t i = result; i >= 0; i-- )
	{
		if(path->buf[i] == '\\')
		{
			path->buf[i] = '\0';
			break;
		}
	}

	return true;
}

bool get_working_directory(struct char_buf *path)
{
	GetCurrentDirectory(path->size, path->buf);

	return GetLastError() == 0;
}

bool set_working_directory(struct char_buf *path)
{
	return SetCurrentDirectory(path->buf);
}

bool open_directory(struct directory_stats *dir, struct char_buf *path)
{
	char search[path->size + 2];
	write_char_buf_format(search, path->size + 2, path->size + 1, "%s\\*", path->buf);

	int32_t num_files = get_file_count(search);

	if(num_files < 0)
		return false;

	if(num_files == 0)
		return true;

	WIN32_FIND_DATA find_data;
	HANDLE handle =  FindFirstFileA(search, &find_data);

	if (handle == INVALID_HANDLE_VALUE)
		return false;

	struct file_stats *files = MALLOC_HEAP(sizeof(struct file_stats) * num_files, 0, "template set file stats");

	int32_t i = 0;

	do
	{
		if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			write_char_buf_count(files[i].name, MAX_FILE_NAME + 1, find_data.cFileName, MAX_FILE_NAME);

			files[i].size = ((size_t)find_data.nFileSizeHigh * ((size_t)MAXDWORD + 1)) + (size_t)find_data.nFileSizeLow;
			files[i].extension[0] = 0;

			char *period = files[i].name;
			char c = 0;

			//file extension is everything after the first period
			while((c = (*period++)))
			{
				if( c == '.' )
				{
					write_char_buf_count(files[i].extension, MAX_FILE_EXTENSION + 1, period, MAX_FILE_EXTENSION);
					break;
				}
			}

			i++;
		}
	} while(i <= num_files && FindNextFileA(handle, &find_data) != 0);

	FindClose(handle);

	if (GetLastError() != ERROR_NO_MORE_FILES)
		return false;

	dir->files = files;
	dir->num_files = num_files;

	return true;
}

void close_directory(struct directory_stats *dir)
{
	if(dir->files)
		MEM_FREE_HEAP(dir->files);
}

static int32_t get_file_count(char *search)
{
	WIN32_FIND_DATA file;
	HANDLE handle =  FindFirstFileA(search, &file);

	if (handle == INVALID_HANDLE_VALUE)
		return -1;

	int32_t count = 0;

	do
	{
		if (!(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			count++;

	} while(FindNextFileA(handle, &file) != 0);

	FindClose(handle);

	if (GetLastError() != ERROR_NO_MORE_FILES)
		return -1;

	return count;
}
