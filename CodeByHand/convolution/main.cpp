#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;

// 灰度转换
int rgb_to_gray(unsigned char r, unsigned char g, unsigned char b) {
    return static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);
}

// 高斯核（5x5, sigma ≈ 1.0）
std::vector<std::vector<float>> get_gaussian_kernel() {
    std::vector<std::vector<float>> kernel = {
            {1,  4,  7,  4, 1},
            {4, 16, 26, 16, 4},
            {7, 26, 41, 26, 7},
            {4, 16, 26, 16, 4},
            {1,  4,  7,  4, 1}
    };
    float sum = 0.0f;
    for (auto& row : kernel)
        for (float val : row)
            sum += val;
    for (auto& row : kernel)
        for (float& val : row)
            val /= sum;
    return kernel;
}

// 卷积（带边界镜像填充）
std::vector<std::vector<int>> convolve(
        const std::vector<std::vector<int>>& gray,
        const std::vector<std::vector<float>>& kernel
) {
    int height = gray.size();
    int width = gray[0].size();
    int ksize = kernel.size();
    int pad = ksize / 2;

    std::vector<std::vector<int>> result(height, std::vector<int>(width, 0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float acc = 0.0f;
            for (int ky = -pad; ky <= pad; ++ky) {
                for (int kx = -pad; kx <= pad; ++kx) {
                    int sy = std::clamp(y + ky, 0, height - 1);
                    int sx = std::clamp(x + kx, 0, width - 1);
                    acc += gray[sy][sx] * kernel[ky + pad][kx + pad];
                }
            }
            result[y][x] = static_cast<int>(acc + 0.5f);
        }
    }

    return result;
}



vector<vector<int>> mytest(vector<vector<int>> gray, vector<vector<float>> kernel) {
    int n = gray.size();
    int m = gray[0].size();
    int pd = kernel.size() / 2;
    vector<vector<int>> res(n, vector<int>(m, 0));
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j++) {
            float t = 0;
            for (int ki = -pd; ki <= pd; ki++) {
                for (int kj = -pd; kj <= pd; kj++) {
                    int x = clamp(i + ki, 0, n - 1);
                    int y = clamp(j + kj, 0, m - 1);
                    t += gray[x][y] * kernel[ki + pd][kj + pd];
                }
            }
            res[i][j] = static_cast<int>(t + 0.5);
        }
    }
    return res;
}








// 可选：保存为 PGM（灰度图）
void save_pgm(const std::string& filename, const std::vector<std::vector<int>>& image) {
    int height = image.size();
    int width = image[0].size();

    std::ofstream fout(filename);
    fout << "P2\n" << width << " " << height << "\n255\n";
    for (auto& row : image) {
        for (auto& val : row) {
            fout << std::clamp(val, 0, 255) << " ";
        }
        fout << "\n";
    }
    fout.close();
}

int main() {
    int width, height, channels;
    unsigned char* data = stbi_load("../convolution/x.jpg", &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "读取 JPG 失败！" << std::endl;
        return -1;
    }

    std::cout << "图像尺寸: " << width << "x" << height << ", 通道数: " << channels << std::endl;

    // 将彩色图转灰度
    std::vector<std::vector<int>> gray(height, std::vector<int>(width, 0));
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * channels;
            unsigned char r = data[idx + 0];
            unsigned char g = data[idx + 1];
            unsigned char b = data[idx + 2];
            gray[y][x] = rgb_to_gray(r, g, b);
        }

    // 卷积处理
    auto kernel = get_gaussian_kernel();
    auto blurred = convolve(gray, kernel);
//    auto blurred = mytest(gray, kernel);

    // 保存为 PGM 格式图像
    save_pgm("blurred.pgm", blurred);
    std::cout << "模糊图像已保存为 blurred.pgm" << std::endl;



    stbi_image_free(data);
    return 0;
}
