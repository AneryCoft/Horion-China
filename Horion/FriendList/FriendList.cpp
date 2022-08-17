#include "FriendList.h"

FriendList g_friend;

FriendList::FriendList() {
}

FriendList::~FriendList() {
}

void FriendList::addPlayerToList(const std::string& name) {
	g_friend.List.push_back(name);
}

std::vector<std::string> FriendList::getList() {
	return g_friend.List;
}

bool FriendList::findPlayer(const std::string& name) {
	for (std::vector<std::string>::iterator it = g_friend.List.begin(); it != g_friend.List.end(); ++it) {
		if (it->find(name) != std::string::npos) {
			return true;
		}
	}
	return false;
}

bool FriendList::removePlayer(const std::string& g) {
	for (std::vector<std::string>::iterator it = g_friend.List.begin(); it != g_friend.List.end(); ++it) {
		std::string copy = *it;
		std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
		if (copy.find(g) != std::string::npos) {
			g_friend.List.erase(it);
			return true;
		}
	}
	return false;
}
