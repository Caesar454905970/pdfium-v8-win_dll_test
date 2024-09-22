#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fpdfview.h>
#include <fpdf_save.h>
#include <fpdf_edit.h>
#include <fpdf_text.h>
#include <fpdf_formfill.h>
#include "fpdf_annot.h"
struct MyFileWriter : public FPDF_FILEWRITE {
    FILE* file;
};

// 用于加载图像数据的函数
std::vector<uint8_t> LoadImageData(const char* image_path) {
    std::ifstream image_file(image_path, std::ios::binary);
    if (!image_file) {
        throw std::runtime_error("Failed to open image file.");
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(image_file)),
                                std::istreambuf_iterator<char>());
}

// 打印表单字段的函数
void PrintFormFields(FPDF_DOCUMENT doc, FPDF_FORMHANDLE form_handle, const std::string& image_path) {
    int page_count = FPDF_GetPageCount(doc);
    for (int i = 0; i < page_count; ++i) {
        FPDF_PAGE page = FPDF_LoadPage(doc, i);
        if (!page) continue;

        // 打印当前这页里面存在表单数量
        int annot_count = FPDFPage_GetAnnotCount(page);
        std::cout << "annot_count: " << annot_count << std::endl;

        for (int j = 0; j < annot_count; ++j) {
            FPDF_ANNOTATION annot = FPDFPage_GetAnnot(page, j);
            if (!annot) continue;

            FPDF_ANNOTATION_SUBTYPE subtype = FPDFAnnot_GetSubtype(annot);
            if (subtype == FPDF_ANNOT_WIDGET) {
                unsigned long name_length = FPDFAnnot_GetFormFieldName(form_handle, annot, nullptr, 0);
                if (name_length > 0) {
                    std::vector<FPDF_WCHAR> name(name_length / sizeof(FPDF_WCHAR));
                    FPDFAnnot_GetFormFieldName(form_handle, annot, name.data(), name_length);
                    std::wstring field_name(name.begin(), name.end() - 1); // 去掉末尾的空字符
                    std::wcout << L"Field Name: " << field_name << std::endl;


                    if(field_name==L"Hosp_logo_imgae"){
                        // 获取表单字段的边界框
                        FS_RECTF rect;
                        FPDFAnnot_GetRect(annot, &rect);
                        double left = rect.left;
                        double top = rect.top;
                        double right = rect.right;
                        double bottom = rect.bottom;
                        double width = right - left;
                        double height = bottom - top;

                        // 创建新图像对象
                        FPDF_PAGEOBJECT new_image = FPDFPageObj_NewImageObj(doc);
                        if (!new_image) {
                            std::cerr << "Failed to create image object." << std::endl;
                            continue;
                        }

                        // 加载图像数据
                        std::vector<uint8_t> image_data = LoadImageData(image_path.c_str());

                        // 设置文件访问结构
                        FPDF_FILEACCESS file_access;
                        file_access.m_FileLen = image_data.size();
                        file_access.m_GetBlock = [](void* param, unsigned long position, unsigned char* buffer, unsigned long size) -> int {
                            std::vector<uint8_t>* image_data = static_cast<std::vector<uint8_t>*>(param);
                            if (position + size > image_data->size()) {
                                return 0;
                            }
                            memcpy(buffer, image_data->data() + position, size);
                            return 1;
                        };
                        file_access.m_Param = &image_data;

                        // 将图像加载到图像对象中
                        if (!FPDFImageObj_LoadJpegFileInline(nullptr, 0, new_image, &file_access)) {
                            std::cerr << "Failed to load image into image object." << std::endl;
                            FPDFPageObj_Destroy(new_image);
                            continue;
                        }


                        // 设置图像矩阵（位置和大小）
                        // 设置图像矩阵（位置和大小），将图像沿 y 轴翻转
                        FPDFImageObj_SetMatrix(new_image, width, 0, 0, -height, left, top + height);
                        // 将新图像对象插入到页面中
                        FPDFPage_InsertObject(page, new_image);


                        // 将新图像对象插入到页面中
                        FPDFPage_InsertObject(page, new_image);

                        // 标记页面内容已更新
                        FPDFPage_GenerateContent(page);

                    }





                }
            }
            //会导致异常报错,注释掉这个
//            FPDFPage_CloseAnnot(annot);
        }

        FPDF_ClosePage(page);
    }
}
/**
 *  填充 表单字段矩形 图片,图片填充满整个表单的大小
 *  把表单的矩形大小，设置为当前表单的大小
 * @return
 */
int main() {
    const char *input_filename = "rep_page1.pdf"; // 输入 PDF 文件
    const std::string image_path = "./reem_logo.png"; // 新图像路径

    // 初始化 PDFium 库
    FPDF_InitLibrary();

    // 打开现有的 PDF 文档
    FPDF_DOCUMENT doc = FPDF_LoadDocument(input_filename, nullptr);
    if (!doc) {
        std::cerr << "Failed to open document: " << input_filename << std::endl;
        FPDF_DestroyLibrary();
        return 1;
    }

    // 创建表单填充环境
    FPDF_FORMFILLINFO form_fill_info = {0};
    form_fill_info.version = 1;
    FPDF_FORMHANDLE form_handle = FPDFDOC_InitFormFillEnvironment(doc, &form_fill_info);

    PrintFormFields(doc, form_handle, image_path);



    // Save the document to a file using FPDF_SaveAsCopy
    FILE* file = fopen(input_filename, "wb");
    if (!file) {
        std::cerr << "Failed to open file: " << input_filename << std::endl;
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    MyFileWriter fileWriter;
    fileWriter.version = 1;
    fileWriter.file = file;
    fileWriter.WriteBlock = [](FPDF_FILEWRITE* pThis, const void* data, unsigned long size) -> int {
        MyFileWriter* writer = static_cast<MyFileWriter*>(pThis);
        return fwrite(data, 1, size, writer->file) == size ? 1 : 0;
    };

    if (!FPDF_SaveAsCopy(doc, &fileWriter, FPDF_NO_INCREMENTAL)) {
        std::cerr << "Failed to save document: " << input_filename << std::endl;
        fclose(file);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    fclose(file);



    // 清理资源
    FPDFDOC_ExitFormFillEnvironment(form_handle);
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();

    std::cout << "PDF successfully processed." << std::endl;
    return 0;
}
