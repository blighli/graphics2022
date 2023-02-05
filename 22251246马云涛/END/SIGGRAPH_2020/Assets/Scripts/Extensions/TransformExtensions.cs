﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class TransformExtensions {

	public static Vector3 GetAxis(this Transform transform, Axis axis) {
		switch(axis) {
			case Axis.XPositive:
			return transform.right;
			case Axis.XNegative:
			return -transform.right;
			case Axis.YPositive:
			return transform.up;
			case Axis.YNegative:
			return -transform.up;
			case Axis.ZPositive:
			return transform.forward;
			case Axis.ZNegative:
			return -transform.forward;
		}
		return Vector3.zero;
	}

	public static Matrix4x4 GetLocalMatrix(this Transform transform, bool unitScale=false) {
		return Matrix4x4.TRS(transform.localPosition, transform.localRotation, unitScale ? Vector3.one : transform.localScale);
	}

	public static Matrix4x4 GetWorldMatrix(this Transform transform, bool unitScale=false) {
		return Matrix4x4.TRS(transform.position, transform.rotation, unitScale ? Vector3.one : transform.lossyScale);
	}

	public static Matrix4x4 GetTransformationMatrix(this Transform transform) {
		return transform.localToWorldMatrix;
	}

	public static void OverridePosition(this Transform transform, Vector3 position) {
		Vector3[] positions = new Vector3[transform.childCount];
		Quaternion[] rotations = new Quaternion[transform.childCount];
		for(int i=0; i<transform.childCount; i++) {
			positions[i] = transform.GetChild(i).position;
			rotations[i] = transform.GetChild(i).rotation;
		}
		transform.position = position;
		for(int i=0; i<transform.childCount; i++) {
			transform.GetChild(i).position = positions[i];
			transform.GetChild(i).rotation = rotations[i];
		}
	}

	public static void OverrideRotation(this Transform transform, Quaternion rotation) {
		Vector3[] positions = new Vector3[transform.childCount];
		Quaternion[] rotations = new Quaternion[transform.childCount];
		for(int i=0; i<transform.childCount; i++) {
			positions[i] = transform.GetChild(i).position;
			rotations[i] = transform.GetChild(i).rotation;
		}
		transform.rotation = rotation;
		for(int i=0; i<transform.childCount; i++) {
			transform.GetChild(i).position = positions[i];
			transform.GetChild(i).rotation = rotations[i];
		}
	}

	public static void OverridePositionAndRotation(this Transform transform, Vector3 position, Quaternion rotation) {
		Vector3[] positions = new Vector3[transform.childCount];
		Quaternion[] rotations = new Quaternion[transform.childCount];
		for(int i=0; i<transform.childCount; i++) {
			positions[i] = transform.GetChild(i).position;
			rotations[i] = transform.GetChild(i).rotation;
		}
		transform.position = position;
		transform.rotation = rotation;
		for(int i=0; i<transform.childCount; i++) {
			transform.GetChild(i).position = positions[i];
			transform.GetChild(i).rotation = rotations[i];
		}
	}

	public static void OverrideLocalPosition(this Transform transform, Vector3 position) {
		Vector3[] positions = new Vector3[transform.childCount];
		Quaternion[] rotations = new Quaternion[transform.childCount];
		for(int i=0; i<transform.childCount; i++) {
			positions[i] = transform.GetChild(i).localPosition;
			rotations[i] = transform.GetChild(i).localRotation;
		}
		transform.localPosition = position;
		for(int i=0; i<transform.childCount; i++) {
			transform.GetChild(i).localPosition = positions[i];
			transform.GetChild(i).localRotation = rotations[i];
		}
	}

	public static void OverrideLocalRotation(this Transform transform, Quaternion rotation) {
		Vector3[] positions = new Vector3[transform.childCount];
		Quaternion[] rotations = new Quaternion[transform.childCount];
		for(int i=0; i<transform.childCount; i++) {
			positions[i] = transform.GetChild(i).localPosition;
			rotations[i] = transform.GetChild(i).localRotation;
		}
		transform.localRotation = rotation;
		for(int i=0; i<transform.childCount; i++) {
			transform.GetChild(i).localPosition = positions[i];
			transform.GetChild(i).localRotation = rotations[i];
		}
	}

	public static void OverrideLocalPositionAndLocalRotation(this Transform transform, Vector3 position, Quaternion rotation) {
		Vector3[] positions = new Vector3[transform.childCount];
		Quaternion[] rotations = new Quaternion[transform.childCount];
		for(int i=0; i<transform.childCount; i++) {
			positions[i] = transform.GetChild(i).localPosition;
			rotations[i] = transform.GetChild(i).localRotation;
		}
		transform.localPosition = position;
		transform.localRotation = rotation;
		for(int i=0; i<transform.childCount; i++) {
			transform.GetChild(i).localPosition = positions[i];
			transform.GetChild(i).localRotation = rotations[i];
		}
	}

}
