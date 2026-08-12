#pragma once

#include <string>

namespace totcad
{

class GDXFModel;

// 从 DXF 文件加载应用使用的图纸模型。
class GDXFLoader final
{
  public:
    bool load(const std::string &filePath, GDXFModel &drawing, std::string *errorMessage = nullptr) const;
};

} // namespace totcad
