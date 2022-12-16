# Dustforce Chaos Mode
Dustforce Chaos Mode is a script/plugin that randomizes aspects of the game continuously, at random intervals.

Inspired by existing Chaos mods from various speedgames.

## Table of contents
- [Contributor's Guide](#guide)
  - [Criteria](#criteria)
  - [Types of Events](#types)
  - [Adding a new Event (short explanation)](#creation-short)
  - [Adding a new Event (detailed explanation)](#creation-detailed)
  - [Testing / Debugging](#testing)

## Contributor's Guide <a id="guide"></a>
**Note: "Event" is the term that we're using for the random effects/modes/things that the script activates, e.g. the "SpawnApples" event, or "MinecraftMode" event.**
### Criteria <a id="criteria"></a>
Most anything goes. Each event has a certain weight that can be attributed to it, determining how likely it is to be picked each interval, so if something is rather extreme/ridiculous it can be justified as an addition just fine, simply by virtue of having it be a low % chance of occurring. That said, there's a handful of things I'd like to avoid:
- Causing motion sickness (so, for example, no continuous camera rotating)
- Crashing the game
- 100% always unavoidably killing a run
- Being overly annoying

It doesn't have to be super fun or extremely hilarious at all. But, if you want to have a flying penis do a drive-by, jizzing dust everywhere, I'm all for it.

### Types of events <a id="types"></a>
**1. Cycle events**

Runs multiple events at the same time, for the same random duration, and deactivates them at the same time, before picking a new list for the next "cycle".

The main loop that the Cycle module runs, consists of:
- Roll a number 1-1000
- Randomly pick a list of events, whose weight is >= the roll.
- Roll a random duration to run the events.
- Call all events' defined `initialize()` methods
- Run the events continuously (i.e. call their `step()` and `draw()` methods, and e.g. other methods like `entity_on_add()`)
- After the duration, call all their `deactivate()` methods
- Repeat the cycle.

**2. Queue events**

Runs events at a faster rate than the Cycle, one at a time. The duration of the events is not tied to the intervals at which new events are picked. In other words, Event `X` can still be running when new Event `Y` is picked. Hence, the name "Queue".

The main loop that the Queue module runs, consists of:
- Roll a number 1-1000
- Randomly pick **one** event, whose weight is >= the roll.
- `initialize()` the event.
- Run the event for as long as `QueueEvent::get_config()` specifies (which could even be `0`, in which case only `initialize()` would be called, or can be `-1` so that the Queue picks a random duration instead)
- `deactivate()` the event.
- Pick a new event at any point and add it to the queue.

### Creating a new event (short explanation) <a id="creation-short"></a>
- Choose Cycle or Queue Event
  - Cycle Events = usually gameplay effects, active for longer durations, balanced around only a few being possible to be active at the same time
  - Queue Events = anything else, can be active for any arbitary duration (currently a duration has to be specified, but we can also code in the possibility for the event itself to signal when it needs to end)

- Get boilerplate code from `/boilerplates`. Your script goes in `/events-cycle|events-queue/events/<your-script>.cpp`,

- Use the `initialize(){}` method as a type of `level_on_start()`, or if your code just needs to run one step, put all your script code here.

- Use whatever methods such as `step()` or `draw()` that your script needs.

- Use the `deactivate(){}` method to revert any effects your script might otherwise permanently have. E.g. if you did `player.scale(2)`, add `player.scale(1)`. For CycleEvents, also reset your script variables here.

- `#include` the file in `events/index.cpp`, add the class instance in the `get_..._events()` function there.
- Alter the returned EventConfig in the `get_config()` method in your script.

  See: `/event-cycle/CycleEvent.cpp` or `/event-queue/QueueEvent.cpp` for the meaning of the arguments.

- For testing your script, see: [testing/debugging](#testing)

### Creating a new event (detailed explanation) <a id="creation-detailed"></a>
- Determine whether you want the event to be part of the Cycle or the Queue. Generally, Cycle events are things that want to be active for longer, usually affecting gameplay/control directly, e.g. scaling the player up/down, changing physics, turning on Minecraft Mode, creating a "letterbox" effect that blocks the sides of the screen, etc. These are specifically balanced around the fact that only a few can be possibly active at the same time.

  Queue Events are anything else, including one-time effects like spawning a bunch of apples, swapping props, replacing/removing geometry, etc. Many can possibly be active at the same time, as the duration can be arbitarily specified. Currently a duration has to be specified, but we can easily code in a way for events to signal themselves to the Queue that they need to be deactivated, and can therefore run for any amount of duration it needs to.

- Copy boilerplate code from `/boilerplates/`, either `CycleEvent.cpp` or `QueueEvent.cpp`.

- Create a new file in `/events-cycle/events/` or `/events-queue/events/`. Paste in the boilerplate code.

- Give the class and constructor whatever name you want, related to what the event does.

- Go to `/event-cycle/events/index.cpp` or `/event-queue/events/index.cpp` and include your file. Put your class instance in the `get_<event-type>_events()` function, in the returned array at the bottom where all the rest are already specified.

- For testing your script, see: [testing/debugging](#testing)

- Specify in the `initialize()` method whatever code the event needs to run the moment it starts. E.g. gathering variables from the scene. Here, you could also already start doing what your event might want to do immediately, like randomizing SFX, spawning apples, etc.

- Do whatever else your events wants to do, as you would in a normal script. In `step()`, `draw()`, `entity_on_add()`, etc. This doesn't always have to be the case, sometimes `initialize()` can contain all the code you need. In this case, you can just remove whatever methods you don't need, like maybe `void draw(){}`. `checkpoint_save()` and `checkpoint_load()` are not available.

*Note that every `CycleEvent` and `QueueEvent` class inherently has access to the seeded Random class methods as part of its base class, with the available methods specified in `/lib/Random.cpp`*

- Once done, think about what needs to happen in `deactivate()`. Here, you revert any changes that may have been otherwise permanent. For example, if in `initialize(){}` I put `player.scale(2)`, in `deactivate(){}` I'll put `player.scale(1)` to revert the effect.

  Additionally, for CycleEvents, `deactivate()` needs to take into account checkpoint reloading, so any variables that the script uses, that are not determined in `initialize()`, should be reset here. For QueueEvents, this is not the case.

  (For context, the way that checkpoint reloading works for the Cycle, is that first the event's `deactivate()` method is called, and then the `initialize()` again, assuming the Event is still in the current active cycle. This is to ensure the event variables are reset.)

- Lastly, determine the Event's configuration in the `QueueEvent|CycleEvent::get_config()` method, which is a method at the top of the class in the boilerplate code. See: `/event-queue/QueueEvent.cpp` or `/event-cycle/CycleEvent.cpp` for an explanation of the arguments, in the respective `Queue|CycleEventConfig` classes.

### Testing/debugging the Event <a id="testing"></a>
- There's an optional test map to use in `/test_level/`, to put in your `level_src`. Here, the `chaos/main.cpp` script is already included (but make sure to recompile of course), and there's a simple map area to test in.
- The main script module has annotations to specify `DebugMode` for both the Cycle and Queue. If on, this will only run events DEBUG events as specified in `/events-cycle|events-queue/events/index.cpp`.

  In the `get_<event-type>_events()` function. First thing you'll see is:
  ```
  if ( DEBUG_MODE ) {
    return array<QueueEvent@> = {
      (...),
    }
  }
  ```
  Class instances in this array are the only ones that will be picked, in debug mode.

- You can turn on/off the Cycle and/or Queue with their respective annotations, as well as "turbo mode", which runs the Cycle and Queue at 0 frame intervals (i.e. they pick new events every frame).
