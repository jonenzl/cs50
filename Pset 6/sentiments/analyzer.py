import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # load positive words
        self.positives = set()
        file = open(positives, "r")
        for line in file:
            # if the line does not start with ';' add the line
            if line.startswith(';') == False:
                self.positives.add(line.rstrip("\n"))
        file.close()
            
        # load negative words
        self.negatives = set()
        file = open(negatives, "r")
        for line in file:
            # if the line does not start with ';' add the line
            if line.startswith(';') == False:
                self.negatives.add(line.rstrip("\n"))
        file.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        sum = 0
        
        # analyze the sentiment of each word, returning 1 if the word is positive, - 1 if negative
        for word in tokens:
            if word.lower() in self.positives:
                sum += 1
            elif word.lower() in self.negatives:
                sum -= 1
            else:
                continue
        
        return sum