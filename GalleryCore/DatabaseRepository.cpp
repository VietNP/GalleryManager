#include "DatabaseRepository.h"
#include <mysql/mysql.h>
#include <iostream>
#include "DatabaseRepository.h"
#include <mysql/mysql.h>

// Thêm dòng này để auto-link thư viện MySQL
#pragma comment(lib, "libmysql.lib")

DatabaseRepository::DatabaseRepository(std::string host, std::string user, std::string password, std::string dbname, unsigned int prt)
    : m_strHost(host), m_strUser(user), m_strPassword(password), m_strDbname(dbname), m_nPort(prt) {
}

std::vector<Image> DatabaseRepository::loadAll() {
    std::vector<Image> list;
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_strDbname.c_str(), m_nPort, NULL, 0)) {
        mysql_close(conn);
        return list;
    }

    if (mysql_query(conn, "SELECT id, filename, filepath FROM images ORDER BY position ASC") == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            Image img;
            img.id = std::stoi(row[0]);
            img.filename = row[1];
            img.filepath = row[2];
            list.push_back(img);
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return list;
}

bool DatabaseRepository::saveAll(const std::vector<Image>& images) {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_strDbname.c_str(), m_nPort, NULL, 0)) {
        return false;
    }

    mysql_query(conn, "DELETE FROM images;"); // Truncate để lưu lại từ RAM xuống DB
    for (size_t i = 0; i < images.size(); ++i) {
        std::string query = "INSERT INTO images (id, filename, filepath, position) VALUES (" +
            std::to_string(images[i].id) + ", '" + images[i].filename + "', '" +
            images[i].filepath + "', " + std::to_string(i) + ");";
        mysql_query(conn, query.c_str());
    }
    mysql_close(conn);
    return true;
}

bool DatabaseRepository::logAction(const std::string& actionType,
    const std::string& payloadRedo,
    const std::string& payloadUndo)
{
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) return false;

    // Kết nối đến Database 'gallery_db' (User: root, Mật khẩu: để trống)
    if (!mysql_real_connect(conn, m_strHost.c_str(), m_strUser.c_str(),
        m_strPassword.c_str(), "gallery_db",
        m_nPort, nullptr, 0))
    {
        mysql_close(conn);
        return false;
    }

    // Ghép câu lệnh INSERT
    std::string query = "INSERT INTO action_logs (action_type, payload_redo, payload_undo) VALUES ('"
        + actionType + "', '"
        + payloadRedo + "', '"
        + payloadUndo + "');";

    int result = mysql_query(conn, query.c_str());
    mysql_close(conn);

    return (result == 0);
}