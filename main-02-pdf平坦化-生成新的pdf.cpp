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
#include <fpdf_flatten.h>

// Flatten operation result macros
#define FLATTEN_FAIL 0
#define FLATTEN_SUCCESS 1
#define FLATTEN_NOTHINGTODO 2

// Flatten modes
#define FLAT_NORMALDISPLAY 0
#define FLAT_PRINT 1

struct MyFileWriter : public FPDF_FILEWRITE {
    FILE *file;
};

void pdf_Flatten(FPDF_DOCUMENT doc, FPDF_FORMHANDLE form_handle) {
    int page_count = FPDF_GetPageCount(doc);
    for (int i = 0; i < page_count; ++i) {
        // 加载当前页面
        FPDF_PAGE page = FPDF_LoadPage(doc, i);
        if (!page) {
            std::cerr << "Failed to load page: " << i << std::endl;
            continue;
        }

        // 平坦化页面上的表单字段和注释
        int flatten_result = FPDFPage_Flatten(page, FLAT_NORMALDISPLAY);
        if (flatten_result == FLATTEN_FAIL) {
            std::cerr << "Failed to flatten page: " << i << std::endl;
        } else if (flatten_result == FLATTEN_SUCCESS) {
            std::cout << "Page " << i << " successfully flattened." << std::endl;
        } else if (flatten_result == FLATTEN_NOTHINGTODO) {
            std::cout << "Nothing to flatten on page: " << i << std::endl;
        }


        // 关闭当前页面
        FPDF_ClosePage(page);
    }
}
/**
 * pdf 平坦化
 * 存在问题,使用edge浏览器打开时,会显示表单字段,虽然不影响打印
 * 需要手动遍历删除表单字段的输入关键字
 * @return
 */
int main() {
    const char *input_filename = "output.pdf";
    const char *output_filename = "output_flattened.pdf";

    // 初始化 PDFium 库
    FPDF_InitLibrary();

    // 打开 PDF 文档
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



    // 将文档中的表单字段平坦化
    pdf_Flatten(doc, form_handle);

    // 保存平坦化后的文档
    FILE *file = fopen(output_filename, "wb");
    if (!file) {
        std::cerr << "Failed to open file: " << output_filename << std::endl;
        FPDF_CloseDocument(doc);
        FPDF_DestroyLibrary();
        return 1;
    }

    MyFileWriter fileWriter;
    fileWriter.version = 1;
    fileWriter.file = file;
    fileWriter.WriteBlock = [](FPDF_FILEWRITE *pThis, const void *data, unsigned long size) -> int {
        MyFileWriter *writer = static_cast<MyFileWriter *>(pThis);
        return fwrite(data, 1, size, writer->file) == size ? 1 : 0;
    };

    if (!FPDF_SaveAsCopy(doc, &fileWriter, FPDF_NO_INCREMENTAL)) {
        std::cerr << "Failed to save flattened document: " << output_filename << std::endl;
    }

    fclose(file);

    // 关闭文档
    FPDF_CloseDocument(doc);

    // 销毁 PDFium 库
    FPDF_DestroyLibrary();

    std::cout << "Document successfully flattened and saved as " << output_filename << std::endl;
    return 0;
}