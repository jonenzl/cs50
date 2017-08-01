from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    # select each stock owned by the user
    portfolio_symbols = db.execute("SELECT shares, symbol FROM portfolio WHERE id = :id", \
                                    id=session["user_id"])
    
    # update each stock price and total
    for portfolio_symbols in portfolio_symbols:
        symbol = portfolio_symbols["symbol"]
        shares = portfolio_symbols["shares"]
        stock = lookup(symbol)
        total = shares * stock["price"]
        
        db.execute("UPDATE portfolio SET price = :price, total = :total \
                    WHERE id = :id AND symbol = :symbol", \
                    price=usd(stock["price"]), total=usd(total), \
                    id=session["user_id"], symbol=symbol)
                    
    # update user's cash in portfolio
    updated_cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    
    # select the user's portfolio to display on the index page
    updated_portfolio = db.execute("SELECT * FROM portfolio WHERE id = :id", id=session["user_id"])
    
    return render_template("index.html", stocks=updated_portfolio, \
                            cash=usd(updated_cash[0]["cash"]))
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via GET
    if request.method == "GET":
        return render_template("buy.html")
        
    else:
        # ensure proper symbol
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("symbol is invalid")
        
        # ensure proper number of shares
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("you must buy a number of shares")
        except:
            return apology("you must buy a number of shares")
            
        # select user's cash
        money = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        
        # check if the user has enough money to make the purchase
        if not money or (float(money[0]["cash"])) < (stock["price"] * shares):
            return apology("you don't have enough money to make the purchase")
            
        # update history
        db.execute("INSERT INTO history (id, symbol, shares, price) \
                    VALUES(:id, :symbol, :shares, :price)", \
                    id=session["user_id"], symbol=stock["symbol"], shares=shares, \
                    price=usd(stock["price"]))
            
        # update user's cash
        db.execute("UPDATE users SET cash = cash - :purchase WHERE id = :id", \
                    id=session["user_id"], purchase=stock["price"] * float(shares))
                    
        # select user shares of the corresponding symbol
        user_shares = db.execute("SELECT shares FROM portfolio \
                                where id = :id AND symbol = :symbol", \
                                id=session["user_id"], symbol=stock["symbol"])
                        
        # if user doesn't has shares of that symbol, create new stock object
        if not user_shares:
            db.execute("INSERT INTO portfolio (id, symbol, name, shares, price, total) \
                        VALUES(:id, :symbol, :name, :shares, :price, :total)", \
                        id=session["user_id"], symbol=stock["symbol"], name=stock["name"], \
                        shares=shares, price=usd(stock["price"]), \
                        total=usd(shares * stock["price"]))
                        
        # Else increment the shares count
        else:
            shares_total = user_shares[0]["shares"] + shares
            db.execute("UPDATE portfolio SET shares=:shares \
                        WHERE id=:id AND symbol=:symbol", \
                        shares=shares_total, id=session["user_id"], \
                        symbol=stock["symbol"])
            
        # return user to home page
        return redirect(url_for("index"))

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    history = db.execute("SELECT * FROM history where id = :id", id=session["user_id"])
    
    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        rows = lookup(request.form.get("symbol"))
        
        if not rows:
            return apology("symbol is invalid")
            
        return render_template("quoted.html", stock=rows)
    
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")
            
        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
            
        # ensure that the two passwords match
        elif request.form.get("password") != request.form.get("passwordagain"):
            return apology("passwords must match")
            
        # insert the new user into the database
        result = db.execute("INSERT INTO users (username, hash) \
                            VALUES(:username, :hash)", \
                            username=request.form.get("username"), \
                            # hash=pwd_context.encrypt(request.form.get("password")))
                            hash=pwd_context.hash(request.form.get("password")))
                            
        if not result:
            return apology("username already exists")
            
        # remember which user is logged in
        session["user_id"] = result
        
        # return user to home page
        return redirect(url_for("index"))
    
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    # if user reached route via GET
    if request.method == "GET":
        return render_template("sell.html")
        
    else:
        # ensure proper symbol
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("symbol is invalid")
        
        # ensure proper number of shares
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("you must sell a number of shares")
        except:
            return apology("you must sell a number of shares")
            
        # select the symbol of the user's shares
        user_shares = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol = :symbol", \
                                    id=session["user_id"], symbol=stock["symbol"])
                                    
        # check if the user has enough shares to sell
        if not user_shares or int(user_shares[0]["shares"]) < shares:
            return apology("you don't have enough shares")
            
        # update the history table with the sale
        db.execute("INSERT INTO history (id, symbol, shares, price) \
                    VALUES(:id, :symbol, :shares, :price)", \
                    id=session["user_id"], symbol=stock["symbol"], shares=-shares, \
                    price=usd(stock["price"]))
                    
        # update the user's cash amount
        db.execute("UPDATE users SET cash = cash + :purchase WHERE id = :id", \
                    id=session["user_id"], purchase=stock["price"] * float(shares))
                    
        # decrement the shares count
        shares_total = user_shares[0]["shares"] - shares
        
        # delete shares from portfolio if the user has sold all the shares
        if shares_total == 0:
            db.execute("DELETE FROM portfolio WHERE id = :id AND symbol = :symbol", \
                        id=session["user_id"], symbol=stock["symbol"])
        
        # otherwise, update the share count in the user's portfolio
        else:
            db.execute("UPDATE portfolio SET shares = :shares WHERE id = :id AND symbol = :symbol", \
                        shares=shares_total, id=session["user_id"], symbol=stock["symbol"])
                        
        # return user to home page
        return redirect(url_for("index"))

@app.route("/change-password", methods=["GET", "POST"])
def change_password():
    """Change password."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
            
        # ensure old password was submitted
        if not request.form.get("oldpassword"):
            return apology("must provide old password")
            
        # ensure new password was submitted
        elif not request.form.get("newpassword"):
            return apology("must provide old password")
            
        # ensure that the two new passwords match
        elif request.form.get("newpassword") != request.form.get("newpasswordagain"):
            return apology("passwords must match")
            
        # update the user's password in the database
        result = db.execute("UPDATE users SET hash = :hash WHERE id = :id", \
                            hash=pwd_context.hash(request.form.get("newpassword")), id=session["user_id"])
                            
        if not result:
            return apology("password was not changed")
            
        # remember which user is logged in
        session["user_id"] = result
        
        # return user to home page
        return redirect(url_for("index"))
    
    else:
        return render_template("change-password.html")