
#include "Room.hpp"

void Room::addPeer(const std::shared_ptr<Peer>& peer) {
  std::lock_guard<std::mutex> guard(m_peerByIdLock);
  m_peerById[peer->getUserId()] = peer;
}

void Room::welcomePeer(const std::shared_ptr<Peer>& peer) {

  auto infoMessage = MessageDto::createShared();
  infoMessage->code = MessageDto::CODE_PEER_INFO;
  infoMessage->peerId = peer->getUserId();
  infoMessage->peerName = peer->getNickname();
  peer->sendMessage(infoMessage);

  auto joinedMessage = MessageDto::createShared();
  joinedMessage->code = MessageDto::CODE_PEER_JOINED;
  joinedMessage->peerId = 0;
  joinedMessage->message = "'" + peer->getNickname() + "' joined room";

  sendMessage(joinedMessage);
}

void Room::goodbyePeer(const std::shared_ptr<Peer>& peer) {

  auto message = MessageDto::createShared();
  message->code = MessageDto::CODE_PEER_LEFT;
  message->peerId = 0;
  message->message = "'" + peer->getNickname() + "' left room";

  sendMessage(message);

}

void Room::removePeerByUserId(v_int64 userId) {
  std::lock_guard<std::mutex> guard(m_peerByIdLock);
  m_peerById.erase(userId);
}

void Room::sendMessage(const MessageDto::ObjectWrapper& message) {
  std::lock_guard<std::mutex> guard(m_peerByIdLock);
  for(auto& pair : m_peerById) {
    pair.second->sendMessage(message);
  }
}