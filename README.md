## 🚀 How it Works

The system operates using a combination of **menu-driven interface, RTC (Real-Time Clock), and interrupts** to ensure timely medicine reminders.

### 📋 Menu-Driven Setup
- The user interacts with the system using a **keypad**.
- A menu is displayed on the **LCD screen**.
- User can:
  - Set current time (RTC)
  - Set medicine reminder time
  - Modify or reset timings

### ⏰ RTC (Real-Time Clock)
- The RTC continuously keeps track of real-world time.
- It runs independently in the background.
- The system regularly checks RTC time against user-set reminder time.

### ⚡ Interrupt Mechanism
- External interrupts (switch/buttons) are used for:
  - Menu navigation
  - Immediate user input detection
- Interrupts ensure:
  - **Fast response**
  - No delay in user interaction
  - Efficient CPU usage (no continuous polling)

### 🔔 Reminder Trigger
- When RTC time matches the set medicine time:
  - An alert is triggered
  - LCD displays reminder message
  - (Optional) buzzer/LED can be activated

### 💡 Key Advantage of Interrupts
- System becomes **real-time responsive**
- Avoids continuous checking (polling)
- Improves performance and efficiency
  
## 🎥 Project Demo Video

[Download Demo Video](./medicine_reminder.mp4)
