﻿using UnityEngine;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

#if UNITY_EDITOR
using UnityEditor;
#endif

public static class Utility {

	private static Collider[] Colliders = new Collider[128];

	public static System.Random RNG;

	public static System.Random GetRNG() {
		if(RNG == null) {
			RNG = new System.Random();
		}
		return RNG;
	}

	public static string GetAssetPath(string guid) {
		#if UNITY_EDITOR
		return AssetDatabase.GUIDToAssetPath(guid);
		#else
		return string.Empty;
		#endif
	}

	public static string GetAssetName(string guid) {
		#if UNITY_EDITOR
		string path = GetAssetPath(guid);
		string id = path.Substring(path.LastIndexOf("/")+1);
		int dot = id.LastIndexOf(".");
		if(dot >= 0) {
			return id.Substring(0, dot);
		} else {
			return string.Empty;
		}
		#else
		return string.Empty;
		#endif
	}

	public static string GetAssetGUID(UnityEngine.Object o) {
		#if UNITY_EDITOR
		if(o == null) {
			return string.Empty;
		}
		return AssetDatabase.AssetPathToGUID(AssetDatabase.GetAssetPath(o));
		#else
		return string.Empty;
		#endif
	}

	public static Vector3 GetClosestPointOverlapSphere(Vector3 center, float radius, LayerMask mask) {
		Collider collider = null;
		return GetClosestPointOverlapSphere(center, radius, mask, out collider);
	}

	public static Vector3 GetClosestPointOverlapSphere(Vector3 center, float radius, LayerMask mask, out Collider collider) {
		Collider[] colliders = Physics.OverlapSphere(center, radius, mask);
		if(colliders.Length == 0) {
			collider = null;
			return center;
		}
		int pivot = 0;
		while(colliders[pivot].isTrigger) {
			pivot++;
			if(pivot == colliders.Length) {
				collider = null;
				return center;
			}
		}
		Vector3 point = colliders[pivot].ClosestPoint(center);
		float x = (point.x-center.x)*(point.x-center.x);
		float y = (point.y-center.y)*(point.y-center.y);
		float z = (point.z-center.z)*(point.z-center.z);
		float min = x*x + y*y + z*z;
		collider = colliders[pivot];
		for(int i=pivot+1; i<colliders.Length; i++) {
			if(!colliders[i].isTrigger) {
				Vector3 candidate = colliders[i].ClosestPoint(center);
				x = (candidate.x-center.x)*(candidate.x-center.x);
				y = (candidate.y-center.y)*(candidate.y-center.y);
				z = (candidate.z-center.z)*(candidate.z-center.z);
				float d = x*x + y*y + z*z;
				if(d < min) {
					point = candidate;
					min = d;
					collider = colliders[i];
				}
			}
		}
		return point;
	}

	public static Vector3 GetClosestPointOverlapBox(Vector3 center, Vector3 halfExtents, Quaternion rotation, LayerMask mask) {
		Collider collider = null;
		return GetClosestPointOverlapBox(center, halfExtents, rotation, mask, out collider);
	}

	public static Vector3 GetClosestPointOverlapBox(Vector3 center, Vector3 halfExtents, Quaternion rotation, LayerMask mask, out Collider collider) {
		Collider[] colliders = Physics.OverlapBox(center, halfExtents, rotation, mask);
		if(colliders.Length == 0) {
			collider = null;
			return center;
		}
		int pivot = 0;
		while(colliders[pivot].isTrigger) {
			pivot++;
			if(pivot == colliders.Length) {
				collider = null;
				return center;
			}
		}
		Vector3 point = colliders[pivot].ClosestPoint(center);
		float x = (point.x-center.x)*(point.x-center.x);
		float y = (point.y-center.y)*(point.y-center.y);
		float z = (point.z-center.z)*(point.z-center.z);
		float min = x*x + y*y + z*z;
		collider = colliders[pivot];
		for(int i=pivot+1; i<colliders.Length; i++) {
			if(!colliders[i].isTrigger) {
				Vector3 candidate = colliders[i].ClosestPoint(center);
				x = (candidate.x-center.x)*(candidate.x-center.x);
				y = (candidate.y-center.y)*(candidate.y-center.y);
				z = (candidate.z-center.z)*(candidate.z-center.z);
				float d = x*x + y*y + z*z;
				if(d < min) {
					point = candidate;
					min = d;
					collider = colliders[i];
				}
			}
		}
		return point;
	}

	public static Quaternion QuaternionEuler(float roll, float pitch, float yaw) {
		roll *= Mathf.Deg2Rad / 2f;
		pitch *= Mathf.Deg2Rad / 2f;
		yaw *= Mathf.Deg2Rad / 2f;

		Vector3 Z = Vector3.forward;
		Vector3 X = Vector3.right;
		Vector3 Y = Vector3.up;

		float sin, cos;

		sin = (float)System.Math.Sin(roll);
		cos = (float)System.Math.Cos(roll);
		Quaternion q1 = new Quaternion(0f, 0f, Z.z * sin, cos);
		sin = (float)System.Math.Sin(pitch);
		cos = (float)System.Math.Cos(pitch);
		Quaternion q2 = new Quaternion(X.x * sin, 0f, 0f, cos);
		sin = (float)System.Math.Sin(yaw);
		cos = (float)System.Math.Cos(yaw);
		Quaternion q3 = new Quaternion(0f, Y.y * sin, 0f, cos);

		return MultiplyQuaternions(MultiplyQuaternions(q1, q2), q3);
	}

	public static Quaternion MultiplyQuaternions(Quaternion q1, Quaternion q2) {
		float x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
		float y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
		float z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
		float w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
		return new Quaternion(x, y, z, w);
	}

	public static void Swap<T>(ref T lhs, ref T rhs) {
        T temp = lhs;
        lhs = rhs;
        rhs = temp;
    }

	public static void Screenshot(string name, int x, int y, int width, int height) {
    	Texture2D tex = new Texture2D(width, height);
		tex.ReadPixels(new Rect(x, y, width, height), 0, 0);
		tex.Apply();
		byte[] bytes = tex.EncodeToPNG();
    	File.WriteAllBytes(name + ".png", bytes);
		Destroy(tex);
	}

	public static void SetFPS(int fps) {
		#if UNITY_EDITOR
		QualitySettings.vSyncCount = 0;
		#else
		QualitySettings.vSyncCount = 0;
		#endif
		Application.targetFrameRate = fps;
	}

	public static System.DateTime GetTimestamp() {
		return System.DateTime.Now;
	}

	public static double GetElapsedTime(System.DateTime timestamp) {
		return (System.DateTime.Now-timestamp).Duration().TotalSeconds;
	}

	public static float Exponential01(float value) {
		float basis = 2f;
		return (Mathf.Pow(basis, Mathf.Clamp(value, 0f, 1f)) - 1f) / (basis-1f);
	}

	//0 = Amplitude, 1 = Frequency, 2 = Shift, 3 = Offset, 4 = Slope, 5 = Time
	public static float LinSin(float a, float f, float s, float o, float m, float t) {
		return a * Mathf.Sin(f * (t - s) * 2f * Mathf.PI) + o + m * t;
	}

	public static float LinSin1(float a, float f, float s, float o, float m, float t) {
		return a * f * Mathf.Cos(f * (t - s) * 2f * Mathf.PI) + m;
	}

	public static float LinSin2(float a, float f, float s, float o, float m, float t) {
		return a * f * f * -Mathf.Sin(f * (t - s) * 2f * Mathf.PI);
	}

	public static float LinSin3(float a, float f, float s, float o, float m, float t) {
		return a * f * f * f * -Mathf.Cos(f * (t - s) * 2f * Mathf.PI);
	}

	public static float Gaussian(float mean, float std, float x) {
		return 1f/(std*Mathf.Sqrt(2f*Mathf.PI)) * Mathf.Exp(-0.5f*(x*x)/(std*std));
	}
	
	public static double Sigmoid(double x) {
		return 1f / (1f + System.Math.Exp(-x));
	}

	public static float Sigmoid(float x) {
		return 1f / (1f + Mathf.Exp(-x));
	}

	public static double TanH(double x) {
		double positive = System.Math.Exp(x);
		double negative = System.Math.Exp(-x);
		return (positive-negative) / (positive+negative);
	}

	public static float TanH(float x) {
		float positive = Mathf.Exp(x);
		float negative = Mathf.Exp(-x);
		return (positive-negative) / (positive+negative);
	}

	public static float Interpolate(float from, float to, float amount) {
		amount = Mathf.Clamp(amount,0f,1f);
		return (1f-amount)*from + amount*to;
	}

	public static double Interpolate(double from, double to, float amount) {
		amount = Mathf.Clamp(amount,0f,1f);
		return (1f-amount)*from + amount*to;
	}

	public static Vector2 Interpolate(Vector2 from, Vector2 to, float amount) {
		amount = Mathf.Clamp(amount,0f,1f);
		return (1f-amount)*from + amount*to;
	}

	public static Vector3 Interpolate(Vector3 from, Vector3 to, float amount) {
		amount = Mathf.Clamp(amount,0f,1f);
		return (1f-amount)*from + amount*to;
	}

	public static Quaternion Interpolate(Quaternion from, Quaternion to, float amount) {
		amount = Mathf.Clamp(amount,0f,1f);
		return Quaternion.Slerp(from,to,amount);
	}

	public static Matrix4x4 Interpolate(Matrix4x4 from, Matrix4x4 to, float amount) {
		// return Matrix4x4.TRS(Interpolate(from.GetPosition(), to.GetPosition(), amount), Interpolate(from.GetRotation(), to.GetRotation(), amount), Interpolate(from.lossyScale, to.lossyScale, amount));
		return Matrix4x4.TRS(Interpolate(from.GetPosition(), to.GetPosition(), amount), Interpolate(from.GetRotation(), to.GetRotation(), amount), Vector3.one);
	}

	public static Matrix4x4 Interpolate(Matrix4x4 from, Matrix4x4 to, float positionAmount, float rotationAmount) {
		return Matrix4x4.TRS(
			Interpolate(from.GetPosition(), to.GetPosition(), positionAmount), 
			Interpolate(from.GetRotation(), to.GetRotation(), rotationAmount),
			Vector3.one
		);
	}

	public static Matrix4x4 Interpolate(Matrix4x4 from, Matrix4x4 to, float positionAmount, float rotationAmount, float scaleAmount) {
		return Matrix4x4.TRS(
			Interpolate(from.GetPosition(), to.GetPosition(), positionAmount), 
			Interpolate(from.GetRotation(), to.GetRotation(), rotationAmount),
			Interpolate(from.lossyScale, to.lossyScale, scaleAmount)
		);
	}

	public static float[] Interpolate(float[] from, float[] to, float amount) {
		if(from.Length != to.Length) {
			Debug.Log("Interpolation not possible.");
			return from;
		}
		float[] result = new float[from.Length];
		for(int i=0; i<result.Length; i++) {
			result[i] = Interpolate(from[i], to[i], amount);
		}
		return result;
	}

	public static Vector3 Lerp(Vector3 from, Vector3 to, Vector3 weights) {
		return new Vector3(
			Mathf.Lerp(from.x, to.x, weights.x),
			Mathf.Lerp(from.y, to.y, weights.y),
			Mathf.Lerp(from.z, to.z, weights.z)
		);
	}

	public static Bounds GetBounds(GameObject instance) {
		Bounds bounds = new Bounds();
		foreach(MeshFilter filter in instance.GetComponentsInChildren<MeshFilter>()) {
			bounds.SetMinMax(Vector3.Min(bounds.min, filter.sharedMesh.bounds.min), Vector3.Max(bounds.max, filter.sharedMesh.bounds.max));
		}
		return bounds;
	}

	public static Vector3 ProjectCollision(Vector3 start, Vector3 end, LayerMask mask) {
		RaycastHit hit;
		if(Physics.Raycast(start, end-start, out hit, Vector3.Magnitude(end-start), mask)) {
			return hit.point;
		}
		return end;
	}

	public static Vector3 ProjectGround(Vector3 position, LayerMask mask) {
		return position.SetY(GetHeight(position, mask));
	}

	public static float GetHeight(Vector3 origin, LayerMask mask, float sky=1000f) {
		RaycastHit hit;
		if(Physics.Raycast(new Vector3(origin.x, sky, origin.z), Vector3.down, out hit, float.PositiveInfinity, mask)) {
			return hit.point.y;
		} else {
			return origin.y;
		}
	}

	public static Vector3 GetNormal(Vector3 origin, LayerMask mask, float sky=1000f) {
		RaycastHit hit;
		if(Physics.Raycast(new Vector3(origin.x, sky, origin.z), Vector3.down, out hit, float.PositiveInfinity, mask)) {
			return hit.normal;
		} else {
			return Vector3.up;
		}
	}

	public static float GetHeight(Vector3 origin, float radius, int samples, int layers, LayerMask mask, float sky=1000f) {
		if(layers == 0) {
			return GetHeight(origin, mask, sky);
		}
		Vector3[] pivots = GetPivots(origin, radius, samples, layers);
		float height = 0f;
		for(int i=0; i<pivots.Length; i++) {
			height += GetHeight(pivots[i], mask, sky);
		}
		return height / pivots.Length;
	}

    public static Vector3 GetNormal(Vector3 origin, float radius, int samples, int layers, LayerMask mask, float sky=1000f) {
		if(layers == 0) {
			GetNormal(origin, mask, sky);
		}
		Vector3[] pivots = GetPivots(origin, radius, samples, layers);
		Vector3 normal = Vector3.zero;
		for(int i=0; i<pivots.Length; i++) {
			normal += GetNormal(pivots[i], mask, sky);
		}
		return (normal / pivots.Length).normalized;
	}

	private static Vector3[] GetPivots(Vector3 origin, float radius, int samples, int layers) {
		Vector3[] pivots = new Vector3[1+layers*samples];
		pivots[0] = origin;
		int index = 1;
		for(int i=1; i<=layers; i++) {
			float r = radius * (float)i / (float)layers;
			float arc = 2*r*Mathf.PI;
			for(int j=0; j<samples; j++) {
				pivots[index] = new Vector3(origin.x + r * Mathf.Sin((float)j/(float)samples * 2f * Mathf.PI), origin.y, origin.z + r * Mathf.Cos((float)j/(float)samples * 2f * Mathf.PI));
				index += 1;
			}
		}
		return pivots;
	}

	public static void Destroy(UnityEngine.Object o, bool allowDestroyingAssets = true) {
		if(Application.isPlaying) {
			GameObject.Destroy(o);
		} else {
			GameObject.DestroyImmediate(o, allowDestroyingAssets);
		}
	}

	public static void SetGUIColor(Color color) {
		GUI.backgroundColor = color;
	}

	public static void ResetGUIColor() {
		SetGUIColor(Color.white);
	}
	
	public static GUIStyle GetFontColor(Color color) {
		GUIStyle style = new GUIStyle();
		style.normal.textColor = color;
		style.hover.textColor = color;
		return style;
	}

	public static bool GUIButton(string label, Color backgroundColor, Color textColor) {
		GUIStyle style = new GUIStyle("Button");
		style.normal.textColor = textColor;
		style.alignment = TextAnchor.MiddleCenter;
		SetGUIColor(backgroundColor);
		bool clicked = GUILayout.Button(label, style);
		ResetGUIColor();
		return clicked;
	}

	public static bool GUIButton(string label, Color backgroundColor, Color textColor, float width, float height) {
		GUIStyle style = new GUIStyle("Button");
		style.normal.textColor = textColor;
		style.alignment = TextAnchor.MiddleCenter;
		SetGUIColor(backgroundColor);
		bool clicked = GUILayout.Button(label, style, GUILayout.Width(width), GUILayout.Height(height));
		ResetGUIColor();
		return clicked;
	}

	public static bool GUIButton(string label, Color backgroundColor, Color textColor, float width) {
		GUIStyle style = new GUIStyle("Button");
		style.normal.textColor = textColor;
		style.alignment = TextAnchor.MiddleCenter;
		SetGUIColor(backgroundColor);
		bool clicked = GUILayout.Button(label, style, GUILayout.Width(width));
		ResetGUIColor();
		return clicked;
	}

	public static Rect GetGUIRect(float x, float y, float width, float height) {
		return new Rect(x*Screen.width, y*Screen.height, width*Screen.width, height*Screen.height);
	}

	// public static float[] Normalise(float[] values) {
	// 	float min = values.Min();
	// 	float max = values.Max();
	// 	float[] result = new float[values.Length];
	// 	for(int i=0; i<values.Length; i++) {
	// 		result[i] = Utility.Normalise(values[i], min, max, 0f, 1f);
	// 	}
	// 	return result;
	// }

	public static void SoftMax(ref float[] values) {
		float frac = 0f;
		for(int i=0; i<values.Length; i++) {
			values[i] = Mathf.Exp(values[i]);
			frac += values[i];
		}
		for(int i=0; i<values.Length; i++) {
			values[i] /= frac;
		}
	}

	public static float Round(float value, int digits) {
		float scale = Mathf.Pow(10, digits);
		return Mathf.Round(value * scale) / scale;
	}

	public static float GaussianValue(float mean, float sigma) {
		if(mean == 0f && sigma == 0f) {
			return 0f;
		}
		// The method requires sampling from a uniform random of (0,1]
		// but Random.NextDouble() returns a sample of [0,1).
		double x1 = 1f - UnityEngine.Random.value;
		double x2 = 1f - UnityEngine.Random.value;
		double y1 = Math.Sqrt(-2.0 * Math.Log(x1)) * Math.Cos(2.0 * Math.PI * x2);
		return (float)(y1 * sigma + mean);
	}

	public static Vector3 GaussianVector3(float mean, float sigma) {
		return new Vector3(GaussianValue(mean, sigma), GaussianValue(mean, sigma), GaussianValue(mean, sigma));
	}

	public static Vector3 GaussianVector3(Vector3 mean, Vector3 sigma) {
		return new Vector3(GaussianValue(mean.x, sigma.x), GaussianValue(mean.y, sigma.y), GaussianValue(mean.z, sigma.z));
	}

	public static Vector3 UniformVector3(Vector3 min, Vector3 max) {
		return new Vector3(UnityEngine.Random.Range(min.x, max.x), UnityEngine.Random.Range(min.y, max.y), UnityEngine.Random.Range(min.z, max.z));
	}

	public static float SignedPhaseUpdate(float from, float to) {
		return -Vector2.SignedAngle(PhaseVector(from), PhaseVector(to)) / 360f;
	}

	public static float PhaseUpdate(float from, float to) {
		return Mathf.Repeat(SignedPhaseUpdate(from, to), 1f);
	}

	public static Vector2 PhaseVector(float phase, float magnitude=1f) {
		phase *= 2f*Mathf.PI;
		return magnitude * new Vector2(Mathf.Sin(phase), Mathf.Cos(phase));
	}

	public static float PhaseValue(Vector2 phase) {
		if(phase.magnitude == 0f) {
			// Debug.LogWarning("Magnitude for computing phase value was zero. Returning zero.");
			return 0f;
		}
		float angle = -Vector2.SignedAngle(Vector2.up, phase.normalized);
		if(angle < 0f) {
			angle = 360f + angle;
		}
		return Mathf.Repeat(angle / 360f, 1f);
	}

	public static float FilterPhaseLinear(float[] values) {
		float[] x = new float[values.Length];
		float[] y = new float[values.Length];
		for(int i=0; i<values.Length; i++) {
			Vector2 v = PhaseVector(values[i]);
			x[i] = v.x;
			y[i] = v.y;
		}
		return Mathf.Repeat(-Vector2.SignedAngle(Vector2.up, new Vector2(x.Mean(), y.Mean()).normalized) / 360f, 1f);
	}

	public static double FilterPhaseLinear(double[] values) {
		float[] x = new float[values.Length];
		float[] y = new float[values.Length];
		for(int i=0; i<values.Length; i++) {
			Vector2 v = PhaseVector((float)values[i]);
			x[i] = v.x;
			y[i] = v.y;
		}
		return Mathf.Repeat(-Vector2.SignedAngle(Vector2.up, new Vector2(x.Mean(), y.Mean()).normalized) / 360f, 1f);
	}

	public static float FilterPhaseGaussian(float[] values) {
		float[] x = new float[values.Length];
		float[] y = new float[values.Length];
		for(int i=0; i<values.Length; i++) {
			Vector2 v = PhaseVector(values[i]);
			x[i] = v.x;
			y[i] = v.y;
		}
		return Mathf.Repeat(-Vector2.SignedAngle(Vector2.up, new Vector2(x.Gaussian(), y.Gaussian()).normalized) / 360f, 1f);
	}

	public static double FilterPhaseGaussian(double[] values) {
		float[] x = new float[values.Length];
		float[] y = new float[values.Length];
		for(int i=0; i<values.Length; i++) {
			Vector2 v = PhaseVector((float)values[i]);
			x[i] = v.x;
			y[i] = v.y;
		}
		return Mathf.Repeat(-Vector2.SignedAngle(Vector2.up, new Vector2(x.Gaussian(), y.Gaussian()).normalized) / 360f, 1f);
	}

	public static Collider GetSelectedCollider() {
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
		RaycastHit hit;
		if(Physics.Raycast(ray, out hit)) {
			return hit.collider;
		} else {
			return null;
		}
	}

	public static T GetMostCommonItem<T>(T[] items) {
		Dictionary<T,int> dic = new Dictionary<T, int>();
		int index = 0;
		T mostCommon = items[0];
		while(mostCommon == null && index<items.Length-1) {
			index += 1;
			mostCommon = items[index];
		}
		if(mostCommon != null) {
			dic.Add(mostCommon, 1);
			for(int i=index+1; i<items.Length; i++) {
				if(items[i] != null) {
					if(dic.ContainsKey(items[i])) {
						dic[items[i]] += 1;
						if(dic[items[i]] > dic[mostCommon]) {
							mostCommon = items[i];
						}
					} else {
						dic.Add(items[i], 1);
					}
				}
			}
		}
		return mostCommon;
	}

	public static GameObject[] Unroll(GameObject[] instances) {
		List<GameObject> candidates = new List<GameObject>();
		for(int k=0; k<instances.Length; k++) {
			Action<GameObject> recursion = null;
			recursion = new Action<GameObject>(instance => {
				candidates.Add(instance);
				for(int i=0; i<instance.transform.childCount; i++) {
					recursion(instance.transform.GetChild(i).gameObject);
				}
			});
			recursion(instances[k]);
		}
		return candidates.ToArray();
	}

	public static Type StringToType(string value) {
		return Type.GetType(value);
	}

	//--------------------------------------------------------------------------
	// This function returns the data filtered. Converted to C# 2 July 2014.
	// Original source written in VBA for Microsoft Excel, 2000 by Sam Van
	// Wassenbergh (University of Antwerp), 6 june 2007.
	//--------------------------------------------------------------------------
	public static double[] Butterworth(double[] indata, double deltaTimeinsec, double CutOff) {
		if (indata == null) return null;
		if (CutOff == 0) return indata;

		double Samplingrate = 1 / deltaTimeinsec;
		long dF2 = indata.Length - 1;        // The data range is set with dF2
		double[] Dat2 = new double[dF2 + 4]; // Array with 4 extra points front and back
		double[] data = indata; // Ptr., changes passed data

		// Copy indata to Dat2
		for (long r = 0; r < dF2; r++) {
			Dat2[2 + r] = indata[r];
		}
		Dat2[1] = Dat2[0] = indata[0];
		Dat2[dF2 + 3] = Dat2[dF2 + 2] = indata[dF2];

		const double pi = 3.14159265358979;
		double wc = Math.Tan(CutOff * pi / Samplingrate);
		double k1 = 1.414213562 * wc; // Sqrt(2) * wc
		double k2 = wc * wc;
		double a = k2 / (1 + k1 + k2);
		double b = 2 * a;
		double c = a;
		double k3 = b / k2;
		double d = -2 * a + k3;
		double e = 1 - (2 * a) - k3;

		// RECURSIVE TRIGGERS - ENABLE filter is performed (first, last points constant)
		double[] DatYt = new double[dF2 + 4];
		DatYt[1] = DatYt[0] = indata[0];
		for (long s = 2; s < dF2 + 2; s++) {
			DatYt[s] = a * Dat2[s] + b * Dat2[s - 1] + c * Dat2[s - 2]
					+ d * DatYt[s - 1] + e * DatYt[s - 2];
		}
		DatYt[dF2 + 3] = DatYt[dF2 + 2] = DatYt[dF2 + 1];

		// FORWARD filter
		double[] DatZt = new double[dF2 + 2];
		DatZt[dF2] = DatYt[dF2 + 2];
		DatZt[dF2 + 1] = DatYt[dF2 + 3];
		for (long t = -dF2 + 1; t <= 0; t++) {
			DatZt[-t] = a * DatYt[-t + 2] + b * DatYt[-t + 3] + c * DatYt[-t + 4]
						+ d * DatZt[-t + 1] + e * DatZt[-t + 2];
		}

		// Calculated points copied for return
		for (long p = 0; p < dF2; p++) {
			data[p] = DatZt[p];
		}

		return data;
	}

}