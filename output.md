A Cross-Platform App for Muscle Tension Detection using MediaPipe
BlazePose

Abstract

The increasing availability of fitness content on social media has made
exercise guidance more accessible, but often lacks objective,
individualized feedback for non-professional users. This work presents a
cross-platform mobile application that estimates relative muscle tension
during exercise using vision-based pose analysis. The system records
video of an end user performing a workout and applies MediaPipe
BlazePose to extract skeletal keypoints, which are used to infer posture
and the direction of external resistance to estimate muscle tension
throughout an exercise. The application will provide real-time feedback
through optional audio cues, visual indicators, and summary diagrams to
improve interpretability for novice gym users. While muscle tension
estimation alone does not fully characterize exercise effectiveness, the
system demonstrates the feasibility of using lightweight pose detection
models to deliver accessible, non-invasive feedback on mobile devices
without specialized equipment.

Motivation

The motivation for creating this app is to create objective, feedback
for beginner gym goers using physics to calculate muscular tension.
Social media provides free fitness information, but this information is
usually without citations. While this app doesn't seek to replace
professional coaching, it will support beginners in making decisions
about which exercises to choose. The app will create a low specs
consumer grade tool for user decision-making regarding exercise
selection.

Requirements

Functional

- Video Recording of exercise

- Generate poses using local MediaPipe

- Estimate resistance vector using poses over time

- Calculate muscle tension based on resistance vector

- Give feedback through diagrams, visual/audio cues

Non-Functional

- Maintain app size below \~100MB target

- Limit CPU usage to prevent battery draining

- Initially work with Kotlin/Android, expand into Swift/iOS

System Overview

1.  Recording

    a.  User starts to record, an audio cue is given when the recording
        begins (so that we don't record extra movements)

    b.  Frames are fed into model to create poses.

2.  Pose Generation

    a.  Each frame is passed into the model, creating a pose. These
        poses are stored for computations, not discarded each frame.

3.  Data Analysis

    a.  After the second repetition, the system calculates the
        resistance path/vector using a sequence of poses.

    b.  Relative muscle tension is estimated on all future reps based on
        joint angles and resistance path.

4.  Feedback

    a.  Visual cues

    b.  Audio cues (optional voice coaching for live improvements)

5.  Post-Workout Summary

    a.  Once recording stops, the system switches to charts of diagrams
        summarizing tension trends. Allow user to crop the workout so
        the tension diagram is based on accurate data.

Implementation

- Kotlin for developing the app

- On-device inference model

- MediaPipe BlazePose

- Event-driven UI updates

> Limitations

- Relative tension, no percentages (we can use a range with color coding
  without giving numerical results)

- Camera angles can reduce accuracy, perhaps need heuristics or users to
  guide the app into determining resistance path

- Inference models can struggle on low-end devices (live-responses can
  be completely optional, giving low-end devices breathing room to
  compute)

Evaluation

Tests:

- Does the app record video?

- Does pose generation run?

- Are resistance vectors and relative tension calculated?

- Does the feedback module work, and can the optional parts be disabled?

- App size
