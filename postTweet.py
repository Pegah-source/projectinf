import sys
sys.path.append('/home/viet1004/.local/lib/python3.8/site-packages')

import tweepy


consumer_key = ''
consumer_secret = ''
access_token = ''
access_token_secret = ''

def OAuth():
	try:
		auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
		auth.set_access_token(access_token, access_token_secret)
		return auth
	except Exception as e:
		return None

oauth = OAuth()
api = tweepy.API(oauth)
with open("message.txt","r") as file:
	text = file.read()
api.update_status(text)
