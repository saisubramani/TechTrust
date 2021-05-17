import pymysql
import sys
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *


class database_userInterface():
    def __init__(self,):
        self.user_name = "root"
        self.password = "sai543DB"
        self.db_name = "admin"
        self.host = "localhost"
        self.window()
        self.connect_db()
        #self.create_database()
        self.show_database()
    def connect_db(self,):
        try:
            self.db_object = pymysql.connect(host=self.host, user=self.user_name,password=self.password)
        except Exception as e:
            print("Connection cant able to establish and cursor object didnot created error : ",e)
        self.db_cursor = self.db_object.cursor()

    def create_database(self,):
        
        database_name = input("Enter the Database name for Admin :")
        
        query = "CREATE DATABASE "+ database_name
        self.db_cursor.execute(query)

    def show_database(self,):
        query = "SHOW DATABASES"
        self.db_cursor.execute(query)
        database_list = self.db_cursor.fetchall()
        for database in database_list:
            print("database_name : ",database)

    def window(self,):
        self.app = QApplication(sys.argv)
        self.w = QWidget()     
        self.b =QPushButton(self.w)
        self.b.setText("ADMINISTRATION DATABASE CREATION")
        self.b.move(50,20)
        self.b.clicked.connect(self.create_database) 
        self.nameLabel1 = QLabel(self.w)
        self.nameLabel1.setText('Enter the Database name for Admin :')
        self.line = QLineEdit(self.w)
        self.line.move(300, 60)
        self.line.resize(200, 32)
        self.nameLabel1.move(20, 65)
        self.w.setGeometry(10,10,900,900)
        self.w.setWindowTitle("TechTrust")
        self.w.show()
        sys.exit(self.app.exec_())


object = database_userInterface()