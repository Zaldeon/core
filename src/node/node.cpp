// std
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include <functional>
#include "../../inc/node.hpp"
#include "../../inc/tree.hpp"

Node::Node(
  int queue,
  unsigned short int port,
  std::map<std::string, Tree>& cm,
  int timeout,
  std::function<bool(std::string)> wd
) : sesh(Create(port, queue)), chains(cm) {
  this->sesh.Criteria(wd);
}

void Node::Open() {
  this->sesh.Open();
}

void Node::Close() {
  this->sesh.Close();
}

void Node::_Await_Stop(int t) {
  try {
    while (!this->alive.empty()) {
      // t => ms
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
  } catch (...) {
    /** the Node was killed :( */
    return;
  }
  exit(0); // kill node
}

void Node::Stop() {
  this->Close();
  int ht; // highest timeout
  for (auto& c: this->alive) {
    ht = (ht < c.get()->timeout) ? c.get()->timeout : ht;
    c.get()->Stop();
  }
  std::thread st(&Node::_Await_Stop, this, ht);
  st.detach();
}

void Node::Next() {
  std::shared_ptr<Peer> np = this->sesh.Accept();
  std::shared_ptr<Conn> nc = std::make_shared<Conn>(Conn(&(this->chains), np, hclc_logic));
  this->alive.push_back(nc);
}

std::shared_ptr<Conn> Node::Contact(std::string msg, std::string ip, int port) {
  std::shared_ptr<Peer> np = this->sesh.Connect(ip+':'+std::to_string(port));
  std::shared_ptr<Conn> nc = std::make_shared<Conn>(Conn(&this->chains, np, hclc_logic));
  this->alive.push_back(nc);
  return nc;
}
