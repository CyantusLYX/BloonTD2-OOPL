import re
import json
import sys

class SmartJSONEncoder(json.JSONEncoder):
    def __init__(self, *args, **kwargs):
        self.threshold = kwargs.pop('threshold', 8)  # 超過此數量的元素才換行
        super(SmartJSONEncoder, self).__init__(*args, **kwargs)
    
    def encode(self, obj):
        if isinstance(obj, list):
            if len(obj) > self.threshold:
                # 大陣列使用一行表示，避免過多行數
                return "[" + ",".join(json.dumps(item) for item in obj) + "]"
        return super(SmartJSONEncoder, self).encode(obj)

def txt_to_json(input_file, output_file):
    levels_data = []

    with open(input_file, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            
            match = re.match(r"Level\s+(\d+):\s+(.*)", line)
            if match:
                level_num = int(match.group(1))
                bloons_str = match.group(2)
                bloons_list = [int(x.strip()) for x in bloons_str.split(',') if x.strip()]
                levels_data.append({
                    "level": level_num,
                    "bloons": bloons_list
                })
    
    # 對關卡進行排序，確保順序從小到大
    levels_data.sort(key=lambda x: x["level"])
    
    # 使用自定義編碼器輸出 JSON
    with open(output_file, 'w', encoding='utf-8') as f_out:
        json.dump(levels_data, f_out, 
                  ensure_ascii=False, 
                  separators=(',', ':'),
                  cls=SmartJSONEncoder)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python round2json.py <input_file> <output_file>")
        sys.exit(1)

    in_file = sys.argv[1]
    out_file = sys.argv[2]
    txt_to_json(in_file, out_file)
    print("轉換完成，結果已輸出到", out_file)