#!/bin/bash

# Travel Planner Chatbot - Run Script
# This script activates the virtual environment and starts the Django server

echo "🗺️  Starting Travel Planner Chatbot..."

# Navigate to project directory
cd "$(dirname "$0")"

# Activate virtual environment
if [ -d "venv" ]; then
    source venv/bin/activate
    echo "✓ Virtual environment activated"
else
    echo "❌ Error: Virtual environment not found. Run 'python3 -m venv venv' first."
    exit 1
fi

# Navigate to Django project
cd travel_chatbot

# Check if database exists
if [ ! -f "db.sqlite3" ]; then
    echo "⚠️  Database not found. Running migrations..."
    python manage.py migrate
fi

# Start server
echo "🚀 Starting Django server on http://localhost:8000"
echo "   Press Ctrl+C to stop"
echo ""
python manage.py runserver
