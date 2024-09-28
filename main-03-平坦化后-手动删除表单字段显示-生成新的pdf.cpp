#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void RemoveLinesWithTarget(const std::string &input_filename, const std::string &output_filename) {
    std::ifstream input_file(input_filename, std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file: " << input_filename << std::endl;
        return;
    }

    std::ofstream output_file(output_filename, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file: " << output_filename << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    // 读取所有行
    while (std::getline(input_file, line)) {
        lines.push_back(line);
    }

    // 遍历并标记要删除的行
    std::vector<bool> to_remove(lines.size(), false);
    for (size_t i = 0; i < lines.size(); ++i) {

        //根据标识符删除不要的行
        if (lines[i].find("Subtype/Widget") != std::string::npos ) {
            // 标记当前行
            to_remove[i] = true;
            // 标记前一行
            if (i > 0) {
                to_remove[i - 1] = true;
            }
            // 标记后一行
            if (i < lines.size() - 1) {
                to_remove[i + 1] = true;
            }
        }
    }

    // 写入未标记的行
    for (size_t i = 0; i < lines.size(); ++i) {
        if (!to_remove[i]) {
            output_file << lines[i] << std::endl;
        }
    }

    input_file.close();
    output_file.close();
}
/**
 * 手动遍历每一行 删除 表单注释字段
 * 输出一个新的Pdf文件
 */
int main() {
    const std::string input_filename = "output_flattened.pdf";
    const std::string output_filename = "output_modified.pdf";

    RemoveLinesWithTarget(input_filename, output_filename);

    std::cout << "Target lines removed and saved to " << output_filename << std::endl;
    return 0;
}
