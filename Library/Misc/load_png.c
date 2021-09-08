#define _CRT_SECURE_NO_WARNINGS

#include <3rdparty/png.h>
#include <stdio.h>
#include <stdlib.h>

//library machine type 'x86' conflicts with target machine type 'x64'
// 依赖于 zlib
// include 和 lib 要匹配，因为这个宏：PNG_LIBPNG_VER_STRING

#define PNG_BYTES_TO_CHECK 4

png_bytep* read_png_file(const char* file_name, int *width, int *height)
{
    png_bytep* row_pointers;
    char header[8];	// 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE* fp = fopen(file_name, "rb");
    if (!fp)
        printf("[read_png_file] File %s could not be opened for reading", file_name);

    fread(header, 1, 4, fp);
    if (png_sig_cmp((png_const_bytep)header, 0, PNG_BYTES_TO_CHECK))
        printf("[read_png_file] File %s is not recognized as a PNG file", file_name);

    /* initialize stuff */
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        printf("[read_png_file] png_create_read_struct failed");

    png_info * info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        printf("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        printf("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    
    png_set_sig_bytes(png_ptr, 4); // = fseek

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    *width = png_get_image_width(png_ptr, info_ptr);

    *height = png_get_image_height(png_ptr, info_ptr);

    int color_type = png_get_color_type(png_ptr, info_ptr);

    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    printf("width %d %d\n", *width, *height);

    row_pointers = png_get_rows(png_ptr, info_ptr);

    //for (y = 0; y < height; y++) {
    //    png_read_rows(png_ptr, &row_pointers[y], NULL, 1);
    //}

    //png_read_end(png_ptr, info_ptr);

    fclose(fp);

    return row_pointers;
}

long png_to_bgra(const char* szPath, int* pnWidth, int* pnHeight, unsigned char** cbData)
{
    FILE* fp = NULL;
    long file_size = 0, pos = 0, mPos = 0;
    int color_type = 0, x = 0, y = 0, block_size = 0;

    png_infop info_ptr;
    png_structp png_ptr;
    png_bytep* row_point = NULL;

    fp = fopen(szPath, "rb");
    if (!fp) {
        return 0;            //文件打开错误则返回 FILE_ERROR
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);        //创建png读取结构
    info_ptr = png_create_info_struct(png_ptr);        //png 文件信息结构
    png_init_io(png_ptr, fp);                //初始化文件 I\O
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);                //读取png文件

    *pnWidth = png_get_image_width(png_ptr, info_ptr);        //获得图片宽度
    *pnHeight = png_get_image_height(png_ptr, info_ptr);        //获得图片高度
    color_type = png_get_color_type(png_ptr, info_ptr);        //获得图片颜色类型
    file_size = (*pnWidth) * (*pnHeight) * 4;                    //计算需要存储RGB(A)数据所需的内存大小
    *cbData = (unsigned char*)malloc(file_size);            //申请所需的内存, 并将传入的 cbData 指针指向申请的这块内容

    row_point = png_get_rows(png_ptr, info_ptr);            //读取RGB(A)数据

    block_size = color_type == 6 ? 4 : 3;                    //根据是否具有a通道判断每次所要读取的数据大小, 具有Alpha通道的每次读4字节, 否则读3字节

    //将读取到的RGB(A)数据按规定格式读到申请的内存中
    for (x = 0; x < *pnHeight; x++)
        for (y = 0; y < *pnWidth * block_size; y += block_size)
        {
            (*cbData)[pos++] = row_point[x][y + 2];        //B
            (*cbData)[pos++] = row_point[x][y + 1];        //G
            (*cbData)[pos++] = row_point[x][y + 0];        //R
            (*cbData)[pos++] = row_point[x][y + 3];        //Alpha
        }

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fclose(fp);

    return file_size;
}
