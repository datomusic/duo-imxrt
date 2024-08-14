import rtmidi
from rtmidi.midiutil import open_midioutput
import time

def find_duo_midi_port():
    for (index, name) in enumerate(rtmidi.MidiOut().get_ports()):
        if "duo" in name.lower():
            print(f"Found {name} connected to MIDI")
            return index
    return None


sequence = [(0,), (2, 1), (3,), (1, 2)]

def main():
    port = find_duo_midi_port()
    output, _portname = open_midioutput(port, use_virtual=False)

    step = 0

    while True:
        time.sleep(0.5)
        notes = sequence[step % len(sequence)]
        step += 1
        for note in notes:
            output.send_message([0x90, note, 120])


if __name__ == "__main__":
    main()


