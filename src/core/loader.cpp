#include "core/loader.hpp"
#include "Util/Logger.hpp"
#include "entities/bloon.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

namespace loader {
std::vector<Bloon::Type> load_bloons(int wave) {
  std::vector<Bloon::Type> result;

  // 開啟 JSON 檔案
  std::ifstream file(RESOURCE_DIR "/bloon.json");
  if (!file.is_open()) {
    std::cerr << "無法開啟 bloon.json 檔案！" << std::endl;
    return result;
  }

  try {
    // 解析 JSON 資料
    nlohmann::json root;
    file >> root;

    // 尋找對應的關卡
    bool found = false;
    for (const auto &level : root) {
      if (level["level"] == wave) {
        found = true;
        LOG_TRACE("LODR  : level {}", wave + 1);

        // 取得該關卡的所有氣球類型
        const auto &bloons = level["bloons"];
        for (const auto &bloon_type : bloons) {
          int type_num = bloon_type.get<int>();

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
            std::cerr << "LODR  : 未知的氣球類型: " << type_num << std::endl;
            break;
          }
        }

        break; // 找到對應關卡後退出循環
      }
    }

    if (!found) {
      std::cerr << "LODR  : 未找到關卡 " << wave << " 的資料！" << std::endl;
    }
  } catch (const nlohmann::json::exception &e) {
    std::cerr << "LODR  : 解析 JSON 檔案失敗: " << e.what() << std::endl;
  }

  return result;
}
} // namespace loader
