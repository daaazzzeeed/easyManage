# easyManage ![C++ pre-release](https://img.shields.io/badge/C%2B%2B-prerelease-blue.svg)

This is a database control system written to automate filling database. 
This is beta version of this application, i still test it and trying to make it better c:

User inteface gives you the ability to add, delete and display information. From now you can easily add, delete and edit data right from TableView thanks to Qt implementation of model/view data representation.
You can switch between tabs in QTabWidget

# Built With
- [Qt](http://www.qt.io/) - user interface and internal logic
- [SQLite Maestro](https://www.sqlmaestro.com/products/sqlite/maestro/) - used to create a database for this project

# Getting started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.
This application requires SQLite client(e.g. SQLite Maestro).

## Installing
Create new database with instructions above or just use '.db' file from this repository.
Download and run Qt Creator, download this project, then open '.pro' file, setup your project. 
In your project change path to '.db' file to match the directory you store the database in.
Now you're ready to go!

# Authors
- Dmitry Petukhov - Initial work/founder

# Acknowledgments
  - You can help me to develop this program or to make it better for your puproses 
  - Feel free to make it look the way you like
