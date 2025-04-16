MIGRATED TO src/irc_cmd :

ircMODE.cpp
ircTOPIC.cpp
ircKICK.cpp
ircJOIN.cpp (incomplete)
pushChannelReplyAll.cpp
pushServerReplyAll.cpp

MIGRATED TO src/irc_response :

errNeedMOreParams.cpp
rplTopicWhoTime.cpp

NOTE:
in case of change from std::string const to std::string const& isn't work, just use "git checkout" and pick up the codes, NO "git revert"