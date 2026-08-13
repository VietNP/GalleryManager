#ifndef DATABASE_REPOSITORY_H
#define DATABASE_REPOSITORY_H

#include "Image.h"
#include <vector>
#include <string>

class DatabaseRepository {
private:
    std::string strHost, strUser, strPassword, strDbname;
    unsigned int nPort;

public:
    DatabaseRepository(std::string host, std::string user, std::string password, std::string dbname, unsigned int port = 3306);

    bool saveAll(const std::vector<Image>& images);
    std::vector<Image> loadAll();
};

#endif // DATABASE_REPOSITORY_H