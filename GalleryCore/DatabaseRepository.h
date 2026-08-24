#ifndef DATABASE_REPOSITORY_H
#define DATABASE_REPOSITORY_H

#include "Image.h"
#include <vector>
#include <string>

class DatabaseRepository {
private:
    std::string m_strHost, m_strUser, m_strPassword, m_strDbname;
    unsigned int m_nPort;

public:
    DatabaseRepository(std::string host, std::string user, std::string password, std::string dbname, unsigned int port = 3306);

    bool saveAll(const std::vector<Image>& images);
    std::vector<Image> loadAll();
    bool logAction(const std::string& actionType, const std::string& payloadRedo, const std::string& payloadUndo);
};

#endif // DATABASE_REPOSITORY_H