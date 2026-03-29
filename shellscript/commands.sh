#!/bin/bash

# 1. Navigate to Desktop
cd ~/Desktop

# 2. Create main directory and subdirectories
mkdir -p "Project Files"/{Docs,Scripts,Backups}

# 3. List contents to verify
ls -R "Project Files"

# 4. Rename oldfile.txt to newfile.txt and move to Docs
mv oldfile.txt "Project Files/Docs/newfile.txt"

# 5. Create an empty file in Scripts
touch "Project Files/Scripts/script1.sh"

# 6. Edit the file (use nano or vim as per preference)
nano "Project Files/Scripts/script1.sh"

# 7. Change ownership of all files inside Project Files
sudo chown -R username:username "Project Files"

# 8. Modify permissions: only owner can execute files in Scripts
chmod u+x "Project Files/Scripts"/*

# 9. Add a new user and set password
sudo adduser newuser
sudo passwd newuser

# 10. Change ownership of specific files for new user
sudo chown newuser:newuser "Project Files/Docs/newfile.txt"

# 11. Display real-time system statistics
top

# 12. Check memory usage
free -h

# 13. Search for a specific word inside a text file
grep "searchword" "Project Files/Docs/newfile.txt"

# 14. Remove an unnecessary file from Backups
rm "Project Files/Backups/unnecessaryfile.txt"

# 15. Create a compressed archive of Project Files (ZIP)
zip -r "Project Files.zip" "Project Files"

# 16. Set the system date
sudo date -s "2026-03-29 15:30:00"

# 17. Modify timestamp of a file or directory
touch -t 202603291530 "Project Files/Docs/newfile.txt"

# 18. Search for a specific word inside a file in Docs
grep "keyword" "Project Files/Docs/newfile.txt"