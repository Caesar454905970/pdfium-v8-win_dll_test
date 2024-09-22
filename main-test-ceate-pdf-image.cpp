#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fpdfview.h>
#include <fpdf_save.h>
#include <fpdf_edit.h>
#include <fpdf_text.h>

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

std::vector<uint8_t> LoadImageData(const char* image_path) {
    std::ifstream image_file(image_path, std::ios::binary);
    if (!image_file) {
        throw std::runtime_error("Failed to open image file.");
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(image_file)),
                                std::istreambuf_iterator<char>());
}

int main() {
    // Define output file name
    const char *output_filename = "output.pdf";  // Output PDF file

    // Initialize PDFium library
    FPDF_InitLibrary();

    // Create a new PDF document
    FPDF_DOCUMENT doc = FPDF_CreateNewDocument();
    if (!doc) {
        std::cerr << "Failed to create new document." << std::endl;
        FPDF_DestroyLibrary();
        return 1;
    }

    // Load the custom Chinese font
    std::vector<uint8_t> font_data;
    try {
        font_data = LoadFontData("./ttfont/AlimamaFangYuan.ttf");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    FPDF_FONT font = FPDFText_LoadFont(doc, font_data.data(), font_data.size(), FPDF_FONT_TRUETYPE, true);
    if (!font) {
        std::cerr << "Failed to load font." << std::endl;
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    // Load the image data
    std::vector<uint8_t> image_data;
    try {
        image_data = LoadImageData("./img/dog.jpeg");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    // Loop to create 4 pages
    for (int i = 0; i < 4; ++i) {
        // Add a new page to the document (A4 size: 595 x 842 points)
        FPDF_PAGE page = FPDFPage_New(doc, i, 595, 842); // A4 size

        // Set the text to be displayed
        std::wstring text = L"这个是一个pdfium生成的pdf-第" + std::to_wstring(i + 1) + L"页";

        // Create a text object
        FPDF_PAGEOBJECT text_object = FPDFPageObj_CreateTextObj(doc, font, 24);
        if (!text_object) {
            std::cerr << "Failed to create text object." << std::endl;
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        // Set the text content (UTF-16LE encoded)
        FPDF_WIDESTRING wide_text = reinterpret_cast<const FPDF_WIDESTRING>(text.c_str());
        FPDFText_SetText(text_object, wide_text);

        // Set the position of the text object
        FPDFPageObj_Transform(text_object, 1, 0, 0, 1, 10, 20);

        // Add the text object to the page
        FPDFPage_InsertObject(page, text_object);

        // Create an image object
        FPDF_PAGEOBJECT image_object = FPDFPageObj_NewImageObj(doc);
        if (!image_object) {
            std::cerr << "Failed to create image object." << std::endl;
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        // Set up the file access structure
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

        // Load the image into the image object
        if (!FPDFImageObj_LoadJpegFileInline(nullptr, 0, image_object, &file_access)) {
            std::cerr << "Failed to load image into image object." << std::endl;
            FPDFPageObj_Destroy(image_object);
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        // Get the bitmap of the image object to retrieve its dimensions
        FPDF_BITMAP bitmap = FPDFImageObj_GetBitmap(image_object);
        if (!bitmap) {
            std::cerr << "Failed to get bitmap from image object." << std::endl;
            FPDFPageObj_Destroy(image_object);
            FPDF_ClosePage(page);
            FPDF_CloseDocument(doc);
            FPDF_DestroyLibrary();
            return 1;
        }

        int width = FPDFBitmap_GetWidth(bitmap);
        int height = FPDFBitmap_GetHeight(bitmap);
        FPDFBitmap_Destroy(bitmap);

        // Set the image matrix (position and size, preserving aspect ratio)
        FPDFImageObj_SetMatrix(image_object, width, 111, 222, height, 333, 444);

        // Add the image object to the page
        FPDFPage_InsertObject(page, image_object);

        // Generate the content of the page
        FPDFPage_GenerateContent(page);

        // Close the page
        FPDF_ClosePage(page);
    }

    // Save the document to a file using FPDF_SaveAsCopy
    FILE* file = fopen(output_filename, "wb");
    if (!file) {
        std::cerr << "Failed to open file: " << output_filename << std::endl;
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
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    fclose(file);

    // Clean up resources
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();

    // Success message
    std::cout << "PDF successfully created." << std::endl;
    return 0;
}