# Read int he file training.csv and print the contents
# in the console
import csv 

with open('training.1600000.processed.noemoticon.csv', 'r') as file:
	reader = csv.reader(file)
	for row in reader:
		# This is the data we want to save
		# timestamp (uuid), username, post content
		data = row[1], row[4], row[5]
		# print(data)
		# Save dato to a new CSV file
		with open('tweets.csv', 'a') as newfile:
			writer = csv.writer(newfile)
			writer.writerow(data)
	newfile.close()
		