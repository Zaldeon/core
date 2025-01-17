#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <functional>

#include "../../../inc/node.hpp"
#include "../../../inc/proto.hpp"
#include "../../../inc/strops.hpp"
#include "../../../inc/crypt.hpp"
#include "../../../inc/tree.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// error handler
json error(int error_code) {
  json ret = {
      {"FLAG", "ERROR"},
      {"CONTENT", error_code}
  };
  return ret;
}


struct hclc : public Protocol {
  private:
  ConnCtx* c = nullptr;   
  
  public:
  json update_chain(json cont); 
  json client_open(std::string);
  json host_open(json);
  json transfer_blocks(json);

  // communication roadmap
  std::map<
    std::string /*prev flag*/, 
    std::function<json(hclc*, json)>
  > next {
    {"COPEN", &hclc::host_open},
    {"HOPEN", &hclc::transfer_blocks},
    {"BLOCKS", &hclc::transfer_blocks},
    {"END", &hclc::update_chain}
  };;
  void Key_Exchange();  
  void ConnH(ConnCtx*, ...) override;  
};
