#include "image_algorithms.hpp"
#include <cmath>
#include <vector>

namespace ImageAlgorithms {

/**
 * @brief 高斯滤波 - 3x3卷积核
 * 
 * 卷积核:
 *   1  2  1
 *   2  4  2
 *   1  2  1
 * 除以16进行归一化
 */
void gaussianFilter(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();
    output.resize(width, height);

    const size_t total = width * height;
    if (total == 0) {
        return;
    }

    // 极小尺寸直接复制返回，避免越界判断
    if (width < 3 || height < 3) {
        std::copy(input.data(), input.data() + total, output.data());
        return;
    }

    // 使用可分离卷积：先进行水平方向卷积（保留未归一化的中间结果），再进行垂直方向卷积。
    std::vector<uint16_t> horizontal_buffer(total, 0);
    const unsigned char* src = input.data();
    uint16_t* tmp = horizontal_buffer.data();

    const size_t stride = width;
    const size_t inner_limit = width - 1; // 最后一列不参与内部卷积

    for (size_t row = 0; row < height; ++row) {
        const unsigned char* row_ptr = src + row * stride;
        uint16_t* tmp_row = tmp + row * stride;

        size_t col = 1;
        const size_t vec_limit = (inner_limit - 1) & ~static_cast<size_t>(3);
        for (; col < vec_limit; col += 4) {
            tmp_row[col + 0] = static_cast<uint16_t>(
                row_ptr[col - 1] + (row_ptr[col + 0] << 1) + row_ptr[col + 1]);
            tmp_row[col + 1] = static_cast<uint16_t>(
                row_ptr[col + 0] + (row_ptr[col + 1] << 1) + row_ptr[col + 2]);
            tmp_row[col + 2] = static_cast<uint16_t>(
                row_ptr[col + 1] + (row_ptr[col + 2] << 1) + row_ptr[col + 3]);
            tmp_row[col + 3] = static_cast<uint16_t>(
                row_ptr[col + 2] + (row_ptr[col + 3] << 1) + row_ptr[col + 4]);
        }
        for (; col < inner_limit; ++col) {
            tmp_row[col] = static_cast<uint16_t>(
                row_ptr[col - 1] + (row_ptr[col] << 1) + row_ptr[col + 1]);
        }
    }

    unsigned char* dst = output.data();

    // 复制顶部和底部边界
    std::copy(src, src + width, dst);
    std::copy(src + (height - 1) * stride, src + height * stride, dst + (height - 1) * stride);
    // 复制左右边界
    for (size_t row = 0; row < height; ++row) {
        dst[row * stride] = src[row * stride];
        dst[row * stride + (width - 1)] = src[row * stride + (width - 1)];
    }

    for (size_t row = 1; row < height - 1; ++row) {
        const uint16_t* prev = tmp + (row - 1) * stride;
        const uint16_t* curr = tmp + row * stride;
        const uint16_t* next = tmp + (row + 1) * stride;
        unsigned char* out_row = dst + row * stride;

        size_t col = 1;
        const size_t vec_limit = (inner_limit - 1) & ~static_cast<size_t>(3);
        for (; col < vec_limit; col += 4) {
            int s0 = prev[col + 0] + (curr[col + 0] << 1) + next[col + 0];
            int s1 = prev[col + 1] + (curr[col + 1] << 1) + next[col + 1];
            int s2 = prev[col + 2] + (curr[col + 2] << 1) + next[col + 2];
            int s3 = prev[col + 3] + (curr[col + 3] << 1) + next[col + 3];
            out_row[col + 0] = static_cast<unsigned char>(s0 >> 4);
            out_row[col + 1] = static_cast<unsigned char>(s1 >> 4);
            out_row[col + 2] = static_cast<unsigned char>(s2 >> 4);
            out_row[col + 3] = static_cast<unsigned char>(s3 >> 4);
        }
        for (; col < inner_limit; ++col) {
            int sum = prev[col] + (curr[col] << 1) + next[col];
            out_row[col] = static_cast<unsigned char>(sum >> 4);
        }
    }
}

/**
 * @brief 幂次变换 - 对比度调整（查找表优化）
 * 
 * 公式: output = 255 * (input/255)^gamma
 * 
 * 由于输入为 0~255 的离散值，这里使用 256 长度的查找表，避免逐像素调用 std::pow。
 */
void powerLawTransformation(const Image& input, Image& output, float gamma) {
    const size_t size = input.size();
    output.resize(input.width(), input.height());

    // 构建查找表：考虑到 gamma 在本实验中固定为 0.5f，仍然按传入参数计算，方便后续扩展。
    unsigned char lut[256];
    const float inv255 = 1.0f / 255.0f;
    for (int v = 0; v < 256; ++v) {
        float normalized = static_cast<float>(v) * inv255;
        float transformed = std::pow(normalized, gamma);
        int out = static_cast<int>(transformed * 255.0f + 0.5f);
        if (out < 0) out = 0;
        if (out > 255) out = 255;
        lut[v] = static_cast<unsigned char>(out);
    }

    const unsigned char* src = input.data();
    unsigned char* dst = output.data();

    size_t i = 0;
    const size_t vec_limit = size & ~static_cast<size_t>(3);
    for (; i < vec_limit; i += 4) {
        dst[i + 0] = lut[src[i + 0]];
        dst[i + 1] = lut[src[i + 1]];
        dst[i + 2] = lut[src[i + 2]];
        dst[i + 3] = lut[src[i + 3]];
    }
    for (; i < size; ++i) {
        dst[i] = lut[src[i]];
    }
}

/**
 * @brief Sobel边缘检测
 * 
 * Gx = [-1 0 1]    Gy = [-1 -2 -1]
 *      [-2 0 2]         [ 0  0  0]
 *      [-1 0 1]         [ 1  2  1]
 */
void sobelEdgeDetection(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();

    output.resize(width, height);

    if (width < 3 || height < 3) {
        std::fill(output.data(), output.data() + output.size(), 0);
        return;
    }

    const unsigned char* src = input.data();
    unsigned char* dst = output.data();
    const size_t stride = width;

    for (size_t i = 1; i < height - 1; ++i) {
        const unsigned char* row_above = src + (i - 1) * stride;
        const unsigned char* row_curr  = src + i * stride;
        const unsigned char* row_below = src + (i + 1) * stride;
        unsigned char* out_row = dst + i * stride;

        for (size_t j = 1; j < width - 1; ++j) {
            int tl = row_above[j - 1];
            int tc = row_above[j];
            int tr = row_above[j + 1];

            int cl = row_curr[j - 1];
            int cr = row_curr[j + 1];

            int bl = row_below[j - 1];
            int bc = row_below[j];
            int br = row_below[j + 1];

            int gx = -tl + tr - 2 * cl + 2 * cr - bl + br;
            int gy = -tl - 2 * tc - tr + bl + 2 * bc + br;

            int mag = std::abs(gx) + std::abs(gy); // 近似幅值，避免 sqrt
            if (mag > 255) mag = 255;
            out_row[j] = static_cast<unsigned char>(mag);
        }
    }

    // 边界置零
    for (size_t j = 0; j < width; ++j) {
        dst[j] = 0;
        dst[(height - 1) * stride + j] = 0;
    }
    for (size_t i = 0; i < height; ++i) {
        dst[i * stride] = 0;
        dst[i * stride + (width - 1)] = 0;
    }
}

/**
 * @brief 图像转置（缓存分块）
 */
void transpose(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();

    output.resize(height, width);

    if (width == 0 || height == 0) {
        return;
    }

    const size_t BLOCK = 32; // 经验块大小，兼顾 L1 缓存与实现简单性
    const unsigned char* src = input.data();
    unsigned char* dst = output.data();

    for (size_t i0 = 0; i0 < height; i0 += BLOCK) {
        for (size_t j0 = 0; j0 < width; j0 += BLOCK) {
            const size_t i_max = std::min(i0 + BLOCK, height);
            const size_t j_max = std::min(j0 + BLOCK, width);

            for (size_t i = i0; i < i_max; ++i) {
                const unsigned char* src_row = src + i * width;
                for (size_t j = j0; j < j_max; ++j) {
                    dst[j * height + i] = src_row[j];
                }
            }
        }
    }
}

/**
 * @brief 均值滤波
 */
void boxFilter(const Image& input, Image& output, int kernel_size) {
    const size_t width = input.width();
    const size_t height = input.height();
    output.resize(width, height);

    if (width == 0 || height == 0) {
        return;
    }

    if (kernel_size <= 1) {
        std::copy(input.data(), input.data() + input.size(), output.data());
        return;
    }

    const int radius = kernel_size / 2;
    const size_t stride = width;
    const size_t integral_stride = width + 1;

    const unsigned char* src = input.data();
    unsigned char* dst = output.data();

    // 构建积分图，额外一行一列前缀零，方便处理边界
    std::vector<uint32_t> integral((width + 1) * (height + 1), 0);
    for (size_t i = 0; i < height; ++i) {
        uint32_t row_sum = 0;
        const unsigned char* row_ptr = src + i * stride;
        uint32_t* integ_row = integral.data() + (i + 1) * integral_stride;
        const uint32_t* integ_row_prev = integral.data() + i * integral_stride;

        for (size_t j = 0; j < width; ++j) {
            row_sum += row_ptr[j];
            integ_row[j + 1] = row_sum + integ_row_prev[j + 1];
        }
    }

    for (size_t i = 0; i < height; ++i) {
        int y0 = static_cast<int>(i) - radius;
        int y1 = static_cast<int>(i) + radius;
        if (y0 < 0) y0 = 0;
        if (y1 >= static_cast<int>(height)) y1 = static_cast<int>(height) - 1;

        for (size_t j = 0; j < width; ++j) {
            int x0 = static_cast<int>(j) - radius;
            int x1 = static_cast<int>(j) + radius;
            if (x0 < 0) x0 = 0;
            if (x1 >= static_cast<int>(width)) x1 = static_cast<int>(width) - 1;

            size_t iy0 = static_cast<size_t>(y0);
            size_t iy1 = static_cast<size_t>(y1);
            size_t ix0 = static_cast<size_t>(x0);
            size_t ix1 = static_cast<size_t>(x1);

            uint32_t sum =
                integral[(iy1 + 1) * integral_stride + (ix1 + 1)] -
                integral[iy0 * integral_stride + (ix1 + 1)] -
                integral[(iy1 + 1) * integral_stride + ix0] +
                integral[iy0 * integral_stride + ix0];

            uint32_t count = static_cast<uint32_t>((ix1 - ix0 + 1) * (iy1 - iy0 + 1));
            dst[i * stride + j] = static_cast<unsigned char>(sum / count);
        }
    }
}

/**
 * @brief 图像旋转90度（顺时针，缓存友好）
 */
void rotate90Clockwise(const Image& input, Image& output) {
    const size_t width = input.width();
    const size_t height = input.height();

    output.resize(height, width);

    if (width == 0 || height == 0) {
        return;
    }

    const size_t BLOCK = 32;
    const unsigned char* src = input.data();
    unsigned char* dst = output.data();

    // 旋转矩阵：dst(x, y) = src(height - 1 - y, x)
    for (size_t i0 = 0; i0 < height; i0 += BLOCK) {
        const size_t i_max = std::min(i0 + BLOCK, height);
        for (size_t j0 = 0; j0 < width; j0 += BLOCK) {
            const size_t j_max = std::min(j0 + BLOCK, width);

            for (size_t i = i0; i < i_max; ++i) {
                const unsigned char* src_row = src + i * width;
                for (size_t j = j0; j < j_max; ++j) {
                    // 源坐标 (i, j) -> 目标坐标 (j, height - 1 - i)
                    dst[j * height + (height - 1 - i)] = src_row[j];
                }
            }
        }
    }
}

/**
 * @brief 计算校验和
 */
unsigned int calcChecksum(const Image& img) {
    unsigned int sum = 0;
    constexpr size_t mod = 1000000007;
    const size_t size = img.size();
    const unsigned char* data = img.data();
    
    for (size_t i = 0; i < size; ++i) {
        sum = (sum + data[i]) % mod;
    }
    
    return sum;
}

/**
 * @brief 比较两个图像
 */
bool compareImages(const Image& img1, const Image& img2, unsigned char tolerance) {
    if (img1.width() != img2.width() || img1.height() != img2.height()) {
        return false;
    }
    
    const size_t size = img1.size();
    for (size_t i = 0; i < size; ++i) {
        int diff = std::abs(static_cast<int>(img1.data()[i]) - static_cast<int>(img2.data()[i]));
        if (diff > tolerance) {
            return false;
        }
    }
    
    return true;
}

} // namespace ImageAlgorithms

