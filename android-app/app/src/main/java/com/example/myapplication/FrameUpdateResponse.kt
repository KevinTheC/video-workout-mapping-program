package com.example.myapplication

data class FrameUpdateResponse(val values: FloatArray){
    enum class Side{
        left,
        right
    }
    enum class Joint{
        shoulder,
        elbow,
        hip,
        knee,
        ankle,
        wrist
    }
    enum class Distance{
        torso,
        shoulderToEar,
        femur
    }
    enum class Plane{
        flexion,
        adduction,
        rotation
    }
    fun getPlanarAngle(side : Side, joint : Joint, plane : Plane) : Float{
        return values[((values.size/2) * side.ordinal) + (joint.ordinal * 3) + plane.ordinal]
    }
    fun getDistance(side : Side, distance: Distance) : Float{
        return values[((values.size/2) * side.ordinal) + 18 + distance.ordinal]
    }
}
