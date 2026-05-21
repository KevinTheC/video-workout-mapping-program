package com.example.myapplication

object BodyState{
    var values: FloatArray = FloatArray(42)
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
    fun getIndexOf(side : Side, joint : Joint, plane : Plane) : Int {
        return ((values.size / 2) * side.ordinal) + (joint.ordinal * 3) + plane.ordinal
    }
    fun getPlanarAngle(side : Side, joint : Joint, plane : Plane) : Float{
        return values[getIndexOf(side, joint, plane)]
    }
    fun getIndexOf(side : Side, distance: Distance) : Int{
        return ((values.size/2) * side.ordinal) + 18 + distance.ordinal
    }
    fun getDistance(side : Side, distance: Distance) : Float{
        return values[getIndexOf(side, distance)]
    }
}