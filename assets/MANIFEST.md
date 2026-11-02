# CE1CHS Resource Manifest

## 资源清单

| 文件 | 来源 | 大小 | 校验结果 | 设备端建议路径 |
|---|---|---:|---|---|
| `assets/HZK16` | <https://raw.githubusercontent.com/aguegu/BitmapFont/master/font/HZK16> | 267,616 bytes | 大小匹配；`D6D0` 偏移 160,928 处的 32 字节与“中”字预期字模完全一致；16 行 MSB-first 点阵预览正确 | `\CE1CHS\HZK16` |
| `ascii8x16.h` | <https://raw.githubusercontent.com/lovyan03/LovyanGFX/master/src/lgfx/Fonts/Ascii8x16.h> | 13,505 bytes | 从 `FontLib8x16[]` 提取 0x00–0x7F，共 2,048 个字模字节；与源数组逐字节一致；文件仅含 ASCII；`A` 字 16 字节验证通过 | 编译进 `\Windows\CE1CHSCore.dll`，不单独部署 |
| `assets/test/test_ascii.txt` | 程序内生成；UTF-8 内容通过 .NET code page 936 转为 GB2312 | 288 bytes | 全部字节位于 ASCII 范围；GB2312 回读及重编码一致 | `\CE1CHS\test\test_ascii.txt` |
| `assets/test/test_cjk.txt` | 程序内生成；UTF-8 内容通过 .NET code page 936 转为 GB2312 | 350 bytes | 无不可映射字符；GB2312 回读及重编码一致 | `\CE1CHS\test\test_cjk.txt` |
| `assets/test/test_mixed.txt` | 程序内生成；UTF-8 内容通过 .NET code page 936 转为 GB2312 | 290 bytes | 无不可映射字符；GB2312 回读及重编码一致 | `\CE1CHS\test\test_mixed.txt` |
| `assets/test/test_control.txt` | 程序内生成；UTF-8 内容通过 .NET code page 936 转为 GB2312 | 199 bytes | GB2312 回读一致；包含 4 个 Tab、5 个 CRLF、2 个单独 LF | `\CE1CHS\test\test_control.txt` |
| `assets/test/test_edge.txt` | 程序内生成；GB2312 基础内容后追加指定原始边界字节 | 161 bytes | 末尾 4 字节精确为 `80 D6 D0 D0`；覆盖孤立非法字节、合法“中”字和末尾截断首字节；无末尾换行 | `\CE1CHS\test\test_edge.txt` |

## SHA-256

| 文件 | SHA-256 |
|---|---|
| `assets/HZK16` | `0A757C641B211419868AF188F4A90B2010FCD7620373073A8BC7C1DD848EEFDF` |
| `ascii8x16.h` | `8F48E1FC06974C245BB54ACF5EC8D2D1B88EEE9209228E80C8A2BC49BD2EC6C8` |
| `assets/test/test_ascii.txt` | `AC78ED60E1D4055E1ACACB5358903A50B782A7F9F67DB441EEA9BF351EA22635` |
| `assets/test/test_cjk.txt` | `4E1C47DE0585E980841AD543C4BEA3D53076BBC376589FFA24922FA65761B83B` |
| `assets/test/test_mixed.txt` | `F54182818AF12D2B91D1FEC7E16AF61F1CBCDE4206BBD4A7C5A00A5919386D5A` |
| `assets/test/test_control.txt` | `E7A6BFA77FFC884D082134507D372E88107402182B9CF1E7F9A1384AC74908B5` |
| `assets/test/test_edge.txt` | `BCA514C3E975DACD46403E31F1903F0DB7867CAB171BCB1C57C76597606BB3F8` |

## 字模定位说明

### HZK16

- 格式：16x16、1bpp、逐行排列、每字 32 字节、高位在先。
- GB2312 区位偏移：`((zone - 1) * 94 + (position - 1)) * 32`。
- “中”字：机内码 `D6D0`，区码 54，位码 48，偏移 160,928。
- 校验字模：`01 00 01 00 01 04 7F FE 41 04 41 04 41 04 41 04 7F FC 41 04 01 00 01 00 01 00 01 00 01 00 01 00`。

### ASCII 8x16

- 每个字符 16 个行字节，每行 8 像素，高位在先。
- 数组保留码位 0x00–0x7F，因此绝对索引为 `code * 16`。
- 以可打印字符码位 0x20 为基址时，相对索引为 `(code - 0x20) * 16`。
- 原始用户首选 URL 返回 404；实际文件取自同一 LovyanGFX 项目的官方 `lovyan03` 仓库路径。

## 边界文件说明

`test_edge.txt` 故意包含非法和截断数据，不能作为“整体必须为合法 GB2312”的校验对象。其合法基础文本之后的字节依次为：

- `80`：孤立非法字节。
- `D6 D0`：合法“中”字。
- `D0`：位于文件末尾、缺少第二字节的 GB2312 首字节。
