import random
import time
import datetime
import calendar
from faker import Faker
from lorem_text import lorem

# Constants
FAKE_PHONE_PREFIX = "+999"

# Aztec calendar "reset": 2012-12-21 00:00:00 UTC
AZTEC_EPOCH = calendar.timegm(datetime.datetime(2012, 12, 21, 0, 0, 0).timetuple())

# Emoji set (~100 emojis)
EMOJIS = [
    # Emotions
    "😀", "😃", "😄", "😁", "😆", "😅", "🤣", "😂", "🙂", "🙃", "😉", "😊", "😇", "😍", "🤩",
    "😘", "😗", "😚", "😋", "😜", "😝", "🤪", "🤓", "😎", "🥳", "😏", "😒", "😞", "😔", "😢",
    "😭", "😤", "😡", "🤬", "🤯", "😳", "🥺", "😬", "🙄", "😴", "🤢", "🤮", "🤧", "😷", "🥶",
    # Animals
    "🐶", "🐱", "🐭", "🐹", "🐰", "🦊", "🐻", "🐼", "🦁", "🐨", "🐯", "🦄", "🐸",
    # Food
    "🍏", "🍎", "🍊", "🍋", "🍌", "🍉", "🍇", "🍓", "🍒", "🍑", "🥭", "🍍", "🥥",
    "🍕", "🍔", "🍟", "🌭", "🍿", "🥓", "🥚", "🍳",
    # Objects
    "💡", "🔦", "📱", "💻", "📷", "📚", "🕹️", "🎮", "🎧", "📺", "📻",
    # Symbols
    "❤️", "💔", "💯", "💥", "✨", "🔥", "🌈", "🎉", "🎁", "🎈", "🧨", "🛑", "✅", "❌", "⚠️"
]

def create_faker(seed):
    faker = Faker()
    Faker.seed(seed)
    random.seed(seed)
    return faker

def generate_contacts(n, faker):
    contacts = []
    for _ in range(n):
        first = faker.first_name()
        last = faker.last_name()
        phone = FAKE_PHONE_PREFIX + "".join(random.choices("0123456789", k=8))
        contacts.append({"first": first, "last": last, "phone": phone})
    return contacts

def generate_message():
    sentence = lorem.sentence()
    words = sentence.rstrip(".").split()

    # 1% chance to MEGA-ify each word
    for i in range(len(words)):
        if random.random() < 0.01:
            words[i] = random.choice(["MEGA65", "MEGAphone"])

    msg = " ".join(words)

    # 40% chance to add 1–6 emojis at start or end
    if random.random() < 0.4:
        emoji_str = "".join(random.choices(EMOJIS, k=random.randint(1, 6)))
        if random.choice([True, False]):
            msg = f"{emoji_str} {msg}"
        else:
            msg = f"{msg} {emoji_str}"

    return msg

def generate_messages(m, contacts):
    output = []
    thread_times = {c["phone"]: AZTEC_EPOCH for c in contacts}
    last_directions = {c["phone"]: random.choice(["rx", "tx"]) for c in contacts}

    for _ in range(m):
        contact = random.choice(contacts)
        phone = contact["phone"]
        prev_dir = last_directions[phone]
        flip = random.random() < 0.75
        direction = "rx" if prev_dir == "tx" else "tx"
        if not flip:
            direction = prev_dir
        last_directions[phone] = direction

        # Advance thread-local timestamp
        thread_times[phone] += random.randint(5, 86400)

        msg = generate_message()
        timestamp = thread_times[phone]
        line = f"MESSAGE{'RX' if direction == 'rx' else 'TX'}:{phone}:{timestamp}:{msg}:"
        output.append(line)

    return output

def generate_dataset(n_contacts, n_messages):
    seed = hash((n_contacts, n_messages))
    faker = create_faker(seed)
    contacts = generate_contacts(n_contacts, faker)
    messages = generate_messages(n_messages, contacts)

    lines = []
    for c in contacts:
        lines.append(f"CONTACT:{c['phone']}:{c['first']}:{c['last']}:")
    lines.extend(messages)
    return lines

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("contacts", type=int, help="Number of fake contacts")
    parser.add_argument("messages", type=int, help="Number of fake messages")
    parser.add_argument("-o", "--output", default="fake_sms_log.txt", help="Output file path")
    args = parser.parse_args()

    lines = generate_dataset(args.contacts, args.messages)
    with open(args.output, "w", encoding="utf-8") as f:
        for line in lines:
            f.write(line + "\n")

    print(f"Generated {args.contacts} contacts and {args.messages} messages into {args.output}")

