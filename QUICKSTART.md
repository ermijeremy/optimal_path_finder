# Quick Start Guide

## 🚀 Fastest Way to Run

If you've already set up the project, simply run:

```bash
./run.sh
```

This script will:
- Activate the virtual environment
- Navigate to the Django project
- Run migrations if needed
- Start the server on http://localhost:8000

## 🔧 If You Get Errors

### Port Already in Use
If you see "Address already in use", kill the existing server:
```bash
pkill -f "manage.py runserver"
```

Then run again:
```bash
./run.sh
```

### CSRF/403 Forbidden Errors
If the chatbot shows errors when sending messages:
1. Make sure you're accessing http://localhost:8000 (not 127.0.0.1)
2. Clear your browser cache
3. Restart the server (Ctrl+C, then `./run.sh`)

### C++ Module Not Found
If you see "No module named 'pathfinding'":
```bash
source venv/bin/activate
python setup.py build_ext --inplace
```

## 📱 Using the Chatbot

Once the server is running, open http://localhost:8000 in your browser.

**Try these commands:**
- "Add route from Paris to London 500km"
- "Find shortest path from Paris to London"
- "Show all cities"
- "Show map statistics"

## 🛑 Stopping the Server

Press `Ctrl+C` in the terminal where the server is running.
