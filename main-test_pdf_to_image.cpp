#include <iostream>
#include <string>
#include <fpdfview.h>
#include <fpdf_formfill.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stbi/stb_image_write.h"


// Helper function to swap color channels from BGRA to RGBA
void SwapBGRAtoRGBA(unsigned char* pixel_data, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 4;
            std::swap(pixel_data[index + 0], pixel_data[index + 2]); // Swap Blue and Red channels
        }
    }
}
int main() {
    const char* input_filename = "input.pdf";  // 输入的 PDF 文件
    FPDF_InitLibrary();

    FPDF_DOCUMENT doc = FPDF_LoadDocument(input_filename, nullptr);
    if (!doc) {
        std::cerr << "Failed to load document: " << input_filename << std::endl;
        FPDF_DestroyLibrary();
        return 1;
    }

    FPDF_FORMFILLINFO form_callbacks = {0};
    form_callbacks.version = 2;
    FPDF_FORMHANDLE form = FPDFDOC_InitFormFillEnvironment(doc, &form_callbacks);

    int page_count = FPDF_GetPageCount(doc);
    for (int i = 0; i < page_count; ++i) {
        FPDF_PAGE page = FPDF_LoadPage(doc, i);
        if (!page) {
            std::cerr << "Failed to load page " << i << " from document: " << input_filename << std::endl;
            continue;
        }

        int width = static_cast<int>(FPDF_GetPageWidth(page));
        int height = static_cast<int>(FPDF_GetPageHeight(page));

        // 增加分辨率比例因子
        int scale_factor = 4; // 例如，4 倍分辨率
        int scaled_width = width * scale_factor;
        int scaled_height = height * scale_factor;

        FPDF_BITMAP bitmap = FPDFBitmap_Create(scaled_width, scaled_height, 0);
        if (!bitmap) {
            std::cerr << "Failed to create bitmap for page " << i << std::endl;
            FPDF_ClosePage(page);
            continue;
        }

        FPDFBitmap_FillRect(bitmap, 0, 0, scaled_width, scaled_height, 0xFFFFFFFF); // White background
        FPDF_RenderPageBitmap(bitmap, page, 0, 0, scaled_width, scaled_height, 0, FPDF_ANNOT);

        unsigned char* pixels = static_cast<unsigned char*>(FPDFBitmap_GetBuffer(bitmap));

        // Swap BGRA to RGBA
        SwapBGRAtoRGBA(pixels, scaled_width, scaled_height);

        std::string output_filename = "output_page_" + std::to_string(i + 1) + ".png";
        if (!stbi_write_png(output_filename.c_str(), scaled_width, scaled_height, 4, pixels, scaled_width * 4)) {
            std::cerr << "Failed to write PNG file: " << output_filename << std::endl;
        } else {
            std::cout << "PDF page " << i + 1 << " successfully converted to PNG: " << output_filename << std::endl;
        }

        FPDFBitmap_Destroy(bitmap);
        FPDF_ClosePage(page);
    }

    FPDFDOC_ExitFormFillEnvironment(form);
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();

    return 0;
}