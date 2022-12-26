#pragma once
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct PositionKey
{
	float time;
	glm::vec3 value;
};

struct RotationKey
{
	float time;
	glm::quat value;
};

struct ScaleKey
{
	float time;
	glm::vec3 value;
};

/*! Animation channel corresponding to one node in the model.
	Note that the keys contain the absolute transform of the bone relative to its parent.
	They are not relative to the bind-pose transform of the bone. */
struct Channel
{
	/*! The node which this channel corresponds to. */
	unsigned int nodeId;

	/*! Position keyframes. */
	std::vector<PositionKey> positionKeys;

	/*! Rotation keyframes. */
	std::vector<RotationKey> rotationKeys;

	/*! Scale keyframes. */
	std::vector<ScaleKey> scaleKeys;
};

/*! Data for a model's single animation. */
struct Animation
{
	/*! Start time of the animation in seconds. */
	float startTime;

	/*! Ending time of the animation in seconds. */
	float endTime;

	/*! Map of node IDs to the corresponding channel index in channels.
		Necessary because not all nodes are animated. */
	std::unordered_map<unsigned int, unsigned int> channelIdMap;

	/*! Vector containing all the channels of the animation. */
	std::vector<Channel> channels;
};

/*! Data specific to a channel for a specific model. */
struct ChannelContext
{
	/*! The last position key we used. */
	unsigned positionKey;

	/*! The last rotation key we used. */
	unsigned rotationKey;

	/*! The last scale key we used. */
	unsigned scaleKey;
};

/*! Data specific to the animation being played.
	Necessary since the same Model object can be shared across multiple entities. */
struct AnimationContext
{
	/*! Key caches. */
	std::unordered_map<unsigned, ChannelContext> channelContexts;
};
