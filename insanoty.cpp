#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;

// =====================
// 1) THE TRIO (and the smart one)
// =====================

class Character {
protected:
    string name;
    vector<string> lines;

public:
    Character(const string& n, const vector<string>& l)
        : name(n), lines(l) {}

    virtual ~Character() = default;

    const string& getName() const { return name; }

    virtual string speak(mt19937& rng) {
        if (lines.empty()) return "...";
        uniform_int_distribution<size_t> dist(0, lines.size() - 1);
        return lines[dist(rng)];
    }
};

class Pedant : public Character {
public:
    Pedant()
        : Character("Pedant", {
            "Actually, that's not technically correct; allow me to clarify.",
            "You misused that word. I must correct you, for the sake of precision.",
            "Let me break this into seventeen sub-points so you truly understand.",
            "Semantically, what you said is inaccurate, though emotionally valid.",
            "If we follow strict logic, your statement collapses at premise one.",
            "You’re confusing correlation with causation again. Classic mistake.",
            "Have you considered the footnote to the appendix of that idea?",
            "I'm obliged to mention the exception case you forgot… all of them."
        }) {}
};

class Moron : public Character {
public:
    Moron()
        : Character("Moron", {
            "I once tried to download more RAM and it almost worked.",
            "If the Earth is round, why do my shoes stay flat on the floor?",
            "I put water in the toaster to clean it. It didn’t end well.",
            "If I can't see Wi-Fi, does that mean I'm invisible to the internet?",
            "I think my brain has a low battery; where’s the charger port?",
            "I deleted the recycle bin so Windows would be more eco-friendly.",
            "I saved my homework by taking a photo of the monitor with my phone.",
            "If 1+1=2, then two pizzas are basically math. I'm a mathematician."
        }) {}
};

class Imbecile : public Character {
public:
    Imbecile()
        : Character("Imbecile", {
            "I’m 100% sure, but also 200% not sure at the same time.",
            "I restarted the router by yelling at it. It blinked, so it worked.",
            "Gravity is just Earth hugging us aggressively, scientifically speaking.",
            "I store my passwords in my memory… and my memory in a sticky note.",
            "I printed an email to make it more digital. Paper is like hard-cloud.",
            "I measured time with a ruler once. The result was very long.",
            "I uninstalled the clock app and now my phone is timeless.",
            "If I close all tabs, my problems disappear. That's computer therapy."
        }) {}
};

// Smart person whose sanity drops over time
class SmartPerson : public Character {
public:
    SmartPerson()
        : Character("Smart", {
            // Reserved, we'll use sanity-based sets instead
        }) {}

    string speak(mt19937& rng, int sanity) {
        vector<string> calm = {
            "Let's try to remain logical about this, please.",
            "I think we can resolve this if we define our terms carefully.",
            "Can we maybe stick to facts for at least one sentence?",
            "I am sure we can find common ground if we slow down."
        };

        vector<string> strained = {
            "I can feel my IQ evaporating with every word you say.",
            "This discussion violates several human rights conventions on thought.",
            "Why are you like this? No, seriously, why?",
            "My brain is buffering. Please… stop… talking…"
        };

        vector<string> broken = {
            "Syntax error in brain.exe… core dumped.",
            "Words… letters… noises… nothing makes sense anymore.",
            "Is this reality or a badly coded simulation of a meeting?",
            "Ha. Haha. Hahaha. LOGIC IS A SOCIAL CONSTRUCT NOW."
        };

        const vector<string>* pool;

        if (sanity > 66) {
            pool = &calm;
        } else if (sanity > 33) {
            pool = &strained;
        } else {
            pool = &broken;
        }

        uniform_int_distribution<size_t> dist(0, pool->size() - 1);
        return (*pool)[dist(rng)];
    }
};

// =====================
// 2) DIALOG + SANITY ANIMATION
// =====================

void animateSanityDrop(int oldSanity, int newSanity) {
    const int barWidth = 40;
    if (newSanity < 0) newSanity = 0;
    if (newSanity > oldSanity) newSanity = oldSanity;

    for (int s = oldSanity; s >= newSanity; --s) {
        int filled = s * barWidth / 100;
        cout << "\rSanity: [";
        for (int i = 0; i < barWidth; ++i) {
            if (i < filled) cout << "#";
            else cout << " ";
        }
        cout << "] " << s << "%   " << flush;
        this_thread::sleep_for(chrono::milliseconds(15));
    }
    cout << "\n";
}

void runArgumentRound(
    Pedant& pedant,
    Moron& moron,
    Imbecile& imbecile,
    SmartPerson& smart,
    mt19937& rng,
    int& sanity
) {
    // Each "round" each character speaks once, in a fixed order.
    cout << pedant.getName()  << ": " << pedant.speak(rng) << "\n";
    cout << moron.getName()   << ": " << moron.speak(rng)  << "\n";
    cout << imbecile.getName()<< ": " << imbecile.speak(rng) << "\n";
    cout << smart.getName()   << ": " << smart.speak(rng, sanity) << "\n";

    // Sanity drops a bit each round, somewhat randomly but always downward.
    uniform_int_distribution<int> dropDist(1, 4);
    int drop = dropDist(rng);
    int oldSanity = sanity;
    sanity = max(0, sanity - drop);

    animateSanityDrop(oldSanity, sanity);
    cout << "\n";
}

// =====================
// 3) MAIN: RUN 5 'DAYS' OF RANDOM ARGUMENTS
// =====================

int main() {
    ios::sync_with_stdio(false);

    // Random generator
    random_device rd;
    mt19937 rng(rd());

    Pedant pedant;
    Moron moron;
    Imbecile imbecile;
    SmartPerson smart;

    int sanity = 100;

    const int daysToSimulate = 5;
    const int argumentsPerDay = 12;  // number of argument rounds per simulated day

    cout << "=== Simulation start: 5 days of endless arguing ===\n\n";

    for (int day = 1; day <= daysToSimulate; ++day) {
        cout << "==========================\n";
        cout << "         DAY " << day << "\n";
        cout << "==========================\n\n";

        for (int round = 1; round <= argumentsPerDay; ++round) {
            if (sanity <= 0) {
                cout << "Smart: I… I can hear the bits screaming. I’m done.\n";
                cout << "\n=== The smart person has completely lost sanity. ===\n";
                return 0;
            }

            cout << "--- Argument round " << round << " ---\n";
            runArgumentRound(pedant, moron, imbecile, smart, rng, sanity);

            // Tiny pause between rounds so the animation feels like a loop
            this_thread::sleep_for(chrono::milliseconds(200));
        }

        cout << "End of day " << day << ". Remaining sanity: " << sanity << "%\n\n";
        // Small pause between days
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "=== 5 simulated days complete. Somehow, a little sanity remains: "
         << sanity << "% ===\n";

    return 0;
}
