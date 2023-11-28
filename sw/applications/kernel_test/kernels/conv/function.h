#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>





/*input 16x16x16*/
static int32_t input[batch_size][C_input][row_input][col_input]=
{
{{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{
       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

},
{

       {00, 01, 02, 03, 04, 05, 06, 07, 8, 9, 10, 11, 12, 13, 14, 15},
       {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
       {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
       {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
       {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
       {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
       {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
       {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
       {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139 ,140, 141, 142, 143},
       {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 ,156, 157, 158, 159},
       {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171 ,172, 173, 174, 175},
       {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187 ,188, 189, 190, 191},
       {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
       {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223}, 
       {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
       {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}



}}};



static int32_t filter[N_filter][C_filter][row_filter][col_filter]=
{
    {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }},
     {{    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    },
    {    {1, 2, 3},
          {4, 5, 6},
          {7, 8, 9}
    }}};

static int32_t outputs[N_output][channel_output][row_output][col_output];

void conv2D()
{
  int32_t l, r, c, k, i, j, w, t;
  int32_t S;
  int32_t coeff;
  int32_t data;
  for (l = 0; l < N_output; l++)
  {
    for (k = 0; k < N_filter; k++)
    {
      for (r = 0; r < row_output; r++)
      {
        for (c = 0; c < col_output; c++)
        {
          S = 0;
          for (w = 0; w < C_filter; w++)
          {
            for (i = -FILT_HALF_x; i <= FILT_HALF_x; i++)
            {
              for (j = -FILT_HALF_y; j <= FILT_HALF_y; j++)
              {
                coeff = filter[k][w][i + FILT_HALF_x][j + FILT_HALF_y];

                data = input[l][w][r + i + FILT_HALF_x][c + j + FILT_HALF_y];
                S += coeff * data;
              }
            }
          }
          outputs[l][k][r][c] = S;
        }
      }
    }
  }




}
