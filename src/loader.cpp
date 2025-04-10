#include "bloon.hpp"
#include "loader.hpp"
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <vector>

namespace loader {
std::vector<Bloon::Type> load_bloons(int wave) {
  std::vector<Bloon::Type> result;

  // 開啟 JSON 檔案
  std::ifstream file(
      RESOURCE_DIR "/bloon.json");
  if (!file.is_open()) {
    std::cerr << "無法開啟 bloon.json 檔案！" << std::endl;
    return result;
  }

  // 解析 JSON 資料
  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  if (!Json::parseFromStream(builder, file, &root, &errs)) {
    std::cerr << "解析 JSON 檔案失敗: " << errs << std::endl;
    return result;
  }

  // 尋找對應的關卡
  bool found = false;
  for (const auto &level : root) {
    if (level["level"].asInt() == wave) {
      found = true;

      // 取得該關卡的所有氣球類型
      const Json::Value &bloons = level["bloons"];
      for (const auto &bloon_type : bloons) {
        int type_num = bloon_type.asInt();

        // 將數字轉換為對應的 Bloon::Type
        switch (type_num) {
        case 1:
          result.push_back(Bloon::Type::red);
          break;
        case 2:
          result.push_back(Bloon::Type::blue);
          break;
        case 3:
          result.push_back(Bloon::Type::green);
          break;
        case 4:
          result.push_back(Bloon::Type::yellow);
          break;
        case 5:
          result.push_back(Bloon::Type::black);
          break;
        case 6:
          result.push_back(Bloon::Type::white);
          break;
        case 7:
          result.push_back(Bloon::Type::lead);
          break;
        case 8:
          result.push_back(Bloon::Type::rainbow);
          break;
        default:
          std::cerr << "未知的氣球類型: " << type_num << std::endl;
          break;
        }
      }

      break; // 找到對應關卡後退出循環
    }
  }

  if (!found) {
    std::cerr << "未找到關卡 " << wave << " 的資料！" << std::endl;
  }

  return result;
}
} // namespace loader