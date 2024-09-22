    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <fpdfview.h>
    #include <fpdf_save.h>
    #include <fpdf_edit.h>
    #include <fpdf_formfill.h>
    #include <fpdf_annot.h>
    /**** 自定义文件写入器结构体 ****/
    struct MyFileWriter : public FPDF_FILEWRITE {
        FILE* file;
    };

    /**** 加载字体数据函数 ****/
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
        const wchar_t *text = L"pdfium";

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


        /**** 生成页面内容 ****/
        FPDFPage_GenerateContent(page);

        /**** 初始化表单填充环境 ****/
        IPDF_JSPLATFORM platform_callbacks = {0};
        platform_callbacks.version = 3;

        FPDF_FORMFILLINFO form_callbacks = {0};
        form_callbacks.version = 2;
        form_callbacks.m_pJsPlatform = &platform_callbacks;

        FPDF_FORMHANDLE form_handle = FPDFDOC_InitFormFillEnvironment(doc, &form_callbacks);
        if (!form_handle) {
            std::cerr << "Failed to initialize form fill environment." << std::endl;
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

     /**** 加载 XFA 表单 ****/
        FPDF_LoadXFA(doc);

        /**** 创建文本输入框（表单字段） ****/
        FPDF_ANNOTATION annot = FPDFPage_CreateAnnot(page, FPDF_ANNOT_WIDGET);
        if (!annot) {
            std::cerr << "Failed to create annotation." << std::endl;
            FPDFDOC_ExitFormFillEnvironment(form_handle);
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        /**** 设置表单字段的位置和大小 ****/
        FS_RECTF rect = {100, 650, 300, 670}; // 左下角 (100, 650), 右上角 (300, 670)
        if (!FPDFAnnot_SetRect(annot, &rect)) {
            std::cerr << "Failed to set annotation rectangle." << std::endl;
            FPDFDOC_ExitFormFillEnvironment(form_handle);
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        /**** 设置表单字段的默认值 ****/
        if (!FPDFAnnot_SetStringValue(annot, "V", reinterpret_cast<FPDF_WIDESTRING>(L"ABC"))) {
            std::cerr << "Failed to set form field value." << std::endl;
            FPDFDOC_ExitFormFillEnvironment(form_handle);
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        /**** 使用 FPDF_SaveAsCopy 将文档保存到文件 ****/
        FILE* file = fopen(output_filename, "wb");
        if (!file) {
            std::cerr << "Failed to open file: " << output_filename << std::endl;
            FPDFDOC_ExitFormFillEnvironment(form_handle);
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
            FPDFDOC_ExitFormFillEnvironment(form_handle);
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        fclose(file);
        /**** 清理资源 ****/



        FPDFDOC_ExitFormFillEnvironment(form_handle);
        FPDF_CloseDocument(doc);

        FPDF_ClosePage(page);
        FPDF_DestroyLibrary();
        /**** 成功消息 ****/
        std::cout << "PDF successfully created." << std::endl;
        return 0;
    }