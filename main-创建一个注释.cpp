#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fpdfview.h>
#include <fpdf_save.h>
#include <fpdf_edit.h>
#include <fpdf_text.h>
#include <fpdf_formfill.h>
#include <fpdf_annot.h>

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
    const char *output_filename = "output.pdf";  // 输出 PDF 文件

    FPDF_InitLibrary();

    FPDF_DOCUMENT doc = FPDF_CreateNewDocument();
    if (!doc) {
        std::cerr << "Failed to create new document." << std::endl;
        FPDF_DestroyLibrary();
        return 1;
    }

    FPDF_PAGE page = FPDFPage_New(doc, 0, 595, 842); // A4 大小

    const wchar_t *text = L"这个是一个pdfium生成的pdf";

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

    FPDF_PAGEOBJECT text_object = FPDFPageObj_CreateTextObj(doc, font, 24);
    if (!text_object) {
        std::cerr << "Failed to create text object." << std::endl;
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    FPDF_WIDESTRING wide_text = reinterpret_cast<const FPDF_WIDESTRING>(text);
    FPDFText_SetText(text_object, wide_text);

    FPDFPageObj_Transform(text_object, 1, 0, 0, 1, 100, 700);
    FPDFPage_InsertObject(page, text_object);

    FPDF_PAGEOBJECT form_field = FPDFPageObj_CreateTextObj(doc, font, 24);
    if (!form_field) {
        std::cerr << "Failed to create form field." << std::endl;
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    const wchar_t *form_field_text = L"Name";
    FPDF_WIDESTRING wide_form_field_text = reinterpret_cast<const FPDF_WIDESTRING>(form_field_text);
    FPDFText_SetText(form_field, wide_form_field_text);

    FPDFPageObj_Transform(form_field, 1, 0, 0, 1, 100, 650);
    FPDFPage_InsertObject(page, form_field);

    /**** 创建注释对象（文本注释） ****/
    FPDF_ANNOTATION annot = FPDFPage_CreateAnnot(page, FPDF_ANNOT_TEXT);
    if (!annot) {
        std::cerr << "Failed to create annotation." << std::endl;
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    /**** 设置注释位置 ****/
    FS_RECTF rect = {100, 600, 200, 620}; // 定义注释矩形区域
    FPDFAnnot_SetRect(annot, &rect);

    /**** 使用FPDFAnnot_SetStringValue设置注释内容 ****/
    const wchar_t* annot_text = L"这是一个注释";

    // 将字符串从UTF-16转换为FPDF_WIDESTRING
    FPDF_WIDESTRING wide_annot_text = reinterpret_cast<const FPDF_WIDESTRING>(annot_text);

    if (!FPDFAnnot_SetStringValue(annot, "Contents", wide_annot_text)) {
        std::cerr << "Failed to set annotation string value." << std::endl;
        FPDFPage_CloseAnnot(annot);
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    FPDFPage_GenerateContent(page);

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
        FPDFPage_CloseAnnot(annot);
        FPDF_ClosePage(page);
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    fclose(file);

    FPDFPage_CloseAnnot(annot);
    FPDF_ClosePage(page);
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();

    std::cout << "PDF successfully created." << std::endl;
    return 0;
}
