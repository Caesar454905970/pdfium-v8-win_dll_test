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

struct MyFileWriter : public FPDF_FILEWRITE
{
    FILE *file;
};

std::vector<uint8_t> LoadFontData(const char *font_path)
{
    std::ifstream font_file(font_path, std::ios::binary);
    if (!font_file)
    {
        throw std::runtime_error("Failed to open font file.");
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(font_file)),
                                std::istreambuf_iterator<char>());
}

void PrintFormFields(FPDF_DOCUMENT doc, FPDF_FORMHANDLE form_handle)
{
    int page_count = FPDF_GetPageCount(doc);
    for (int i = 0; i < page_count; ++i)
    {
        FPDF_PAGE page = FPDF_LoadPage(doc, i);
        if (!page)
            continue;

        int annot_count = FPDFPage_GetAnnotCount(page);
        for (int j = 0; j < annot_count; ++j)
        {
            FPDF_ANNOTATION annot = FPDFPage_GetAnnot(page, j);
            if (!annot)
                continue;

            FPDF_ANNOTATION_SUBTYPE subtype = FPDFAnnot_GetSubtype(annot);
            if (subtype == FPDF_ANNOT_WIDGET)
            {
                unsigned long name_length = FPDFAnnot_GetFormFieldName(form_handle, annot, nullptr, 0);
                if (name_length > 0)
                {
                    std::vector<FPDF_WCHAR> name(name_length / sizeof(FPDF_WCHAR));
                    FPDFAnnot_GetFormFieldName(form_handle, annot, name.data(), name_length);
                    std::wstring field_name(name.begin(), name.end() - 1); // 去掉末尾的空字符
                    std::wcout << L"Save Field Name: " << field_name << std::endl;

                    // 判断是否等于 "LCQ"
                    if (field_name == L"LCQ")
                    {
                        std::wcout << L"Field Name is LCQ" << std::endl;
                    }
                }
            }
            FPDFPage_CloseAnnot(annot);
        }
        FPDF_ClosePage(page);
    }
}

int main()
{
    /**** 定义输入文件名 ****/
    const char *input_filename = "output.pdf"; // 输入 PDF 文件

    /**** 初始化 PDFium 库 ****/
    FPDF_InitLibrary();

    /**** 打开现有的 PDF 文档 ****/
    FPDF_DOCUMENT doc = FPDF_LoadDocument(input_filename, nullptr);
    if (!doc)
    {
        std::cerr << "Failed to open document: " << input_filename << std::endl;
        FPDF_DestroyLibrary();
        return 1;
    }

    /**** 创建表单填充环境 ****/
    FPDF_FORMFILLINFO form_fill_info = {0};
    form_fill_info.version = 1;
    FPDF_FORMHANDLE form_handle = FPDFDOC_InitFormFillEnvironment(doc, &form_fill_info);

    /**** 打印所有表单字段 ****/
    PrintFormFields(doc, form_handle);

    /**** 使用 FPDF_SaveAsCopy 将文档保存到文件 ****/
    FILE *file = fopen(input_filename, "wb");
    if (!file)
    {
        std::cerr << "Failed to open file: " << input_filename << std::endl;
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    MyFileWriter fileWriter;
    fileWriter.version = 1;
    fileWriter.file = file;
    fileWriter.WriteBlock = [](FPDF_FILEWRITE *pThis, const void *data, unsigned long size) -> int
    {
        MyFileWriter *writer = static_cast<MyFileWriter *>(pThis);
        return fwrite(data, 1, size, writer->file) == size ? 1 : 0;
    };

    if (!FPDF_SaveAsCopy(doc, &fileWriter, FPDF_NO_INCREMENTAL))
    {
        std::cerr << "Failed to save document: " << input_filename << std::endl;
        fclose(file);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    fclose(file);

    /**** 清理资源 ****/
    FPDFDOC_ExitFormFillEnvironment(form_handle);
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();

    /**** 成功消息 ****/
    std::cout << "PDF successfully updated." << std::endl;
    return 0;
}