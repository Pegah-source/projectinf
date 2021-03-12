import sys
sys.path.append('/home/viet1004/.local/lib/python3.8/site-packages')

import tweepy


consumer_key = 'jRy6vDD8Ro20e6eoKznUZWyhl'
consumer_secret = 'hdROeu1S8OfrCHWNj9iHAjX12tRI6ZxfoYIRYz3gDBpBGj1SSQ'
access_token = '1364598542401220609-IrDcUpPoiw9sbRwIjx0USTIx7cMSPx'
access_token_secret = 'XPyJqrmbyY2uK00Ch5c1YxLjSJA2YvR3vHck7wmoZuGz1'

search_word = "#klmdcatadbac"
#search_word = "So"

def OAuth():
	try:
		auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
		auth.set_access_token(access_token, access_token_secret)
		return auth
	except Exception as e:
		return None

oauth = OAuth()
api = tweepy.API(oauth)

tweets = tweepy.Cursor(api.search(), q = search_word, lang = "en").items(200)

#with open("TweetDownload.txt", "w") as filetxt:
temp = []
for tweet in tweets:
	temp.insert(0,tweet.text)
	
with open("TweetDownload.txt", "w") as filetxt:
	for tweet in temp:
		filetxt.write(tweet)
		filetxt.write("\n")

