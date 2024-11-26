# Read int he file training.csv and print the contents
# in the console
import csv 

with open('training.1600000.processed.noemoticon.csv', 'r') as file:
	reader = csv.reader(file)
	i = 0
	for row in reader:
		# This is the data we want to save
		# Just the post content, line number is extractable
		# Cannot use the timestamp because there are duplicates
		data = [row[5]]
		# Save dato to a new CSV file
		with open('tweets.csv', 'a') as newfile:
			writer = csv.writer(newfile)
			writer.writerow(data)
		i += 1
	newfile.close()
		