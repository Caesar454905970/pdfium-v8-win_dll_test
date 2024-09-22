#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fpdfview.h>
#include <fpdf_save.h>
#include <fpdf_edit.h>
#include <fpdf_text.h>
#include <fpdf_formfill.h>

struct MyFileWriter : public FPDF_FILEWRITE {
    FILE* file;
};

std::vector<uint8_t> LoadFontData(const char* font_path) {
    std::ifstream font_file(font_path, std::ios::binary);
    if (!font_file) {
        throw std::runtime_error("Failed to open font file.");
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(font_file)),
                                std::istreambuf_iterator<char>());
}

int main() {
    /**** 定义输出文件名 ****/
    const char *output_filename = "output.pdf";  // 输出 PDF 文件

    /**** 初始化 PDFium 库 ****/
    FPDF_InitLibrary();

    /**** 创建一个新的 PDF 文档 ****/
    FPDF_DOCUMENT doc = FPDF_CreateNewDocument();
    if (!doc) {
        std::cerr << "Failed to create new document." << std::endl;
        FPDF_DestroyLibrary();
        return 1;
    }

    /**** 向文档添加一个新页面（A4 大小：595 x 842 点） ****/
    FPDF_PAGE page = FPDFPage_New(doc, 0, 595, 842); // A4 大小

    /**** 设置要显示的文本 ****/
    const wchar_t *text = L"这个是一个pdfium生成的pdf";

    /**** 加载自定义中文字体 ****/
    std::vector<uint8_t> font_data;
    try {
        font_data = LoadFontData("./ttfont/AlimamaFangYuan.ttf");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    FPDF_FONT font = FPDFText_LoadFont(doc, font_data.data(), font_data.size(), FPDF_FONT_TRUETYPE, true);
    if (!font) {
        std::cerr << "Failed to load font." << std::endl;
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    /**** 创建文本对象 ****/
    FPDF_PAGEOBJECT text_object = FPDFPageObj_CreateTextObj(doc, font, 24);
    if (!text_object) {
        std::cerr << "Failed to create text object." << std::endl;
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    /**** 设置文本内容（UTF-16LE 编码） ****/
    FPDF_WIDESTRING wide_text = reinterpret_cast<const FPDF_WIDESTRING>(text);
    FPDFText_SetText(text_object, wide_text);

    /**** 设置文本对象的位置 ****/
    FPDFPageObj_Transform(text_object, 1, 0, 0, 1, 100, 700);

    /**** 将文本对象添加到页面 ****/
    FPDFPage_InsertObject(page, text_object);

    /**** 创建表单字段（文本字段） ****/
    FPDF_PAGEOBJECT form_field = FPDFPageObj_CreateTextObj(doc, font, 24);
    if (!form_field) {
        std::cerr << "Failed to create form field." << std::endl;
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    /**** 设置表单字段内容（UTF-16LE 编码） ****/
    const wchar_t *form_field_text = L"Name";
    FPDF_WIDESTRING wide_form_field_text = reinterpret_cast<const FPDF_WIDESTRING>(form_field_text);
    FPDFText_SetText(form_field, wide_form_field_text);

    /**** 设置表单字段的位置 ****/
    FPDFPageObj_Transform(form_field, 1, 0, 0, 1, 100, 650);

    /**** 将表单字段添加到页面 ****/
    FPDFPage_InsertObject(page, form_field);

    /**** 生成页面内容 ****/
    FPDFPage_GenerateContent(page);

    /**** 使用 FPDF_SaveAsCopy 将文档保存到文件 ****/
    FILE* file = fopen(output_filename, "wb");
    if (!file) {
        std::cerr << "Failed to open file: " << output_filename << std::endl;
        FPDF_ClosePage(page);
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
        std::cerr << "Failed to save document: " << output_filename << std::endl;
        fclose(file);
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    fclose(file);

    /**** 清理资源 ****/
    FPDF_ClosePage(page);
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();

    /**** 成功消息 ****/
    std::cout << "PDF successfully created." << std::endl;
    return 0;
}