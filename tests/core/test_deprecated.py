import pytest


class TestDeprecated:
    def test_old_import(self):
        with pytest.warns(DeprecationWarning, match="Stormpy was restructured"):
            import stormpy.all

        # Use the SparseMatrixBuilder for constructing the transition matrix
        builder = stormpy.SparseMatrixBuilder(rows=0, columns=0, entries=0, force_dimensions=False, has_custom_row_grouping=False)

        # New Transition from state 0 to target state 1 with probability 0.5
        builder.add_next_value(0, 1, 0.5)
        builder.add_next_value(0, 2, 0.5)
        builder.add_next_value(1, 3, 0.5)
        builder.add_next_value(1, 4, 0.5)
        builder.add_next_value(2, 5, 0.5)
        builder.add_next_value(2, 6, 0.5)
        builder.add_next_value(3, 7, 0.5)
        builder.add_next_value(3, 1, 0.5)
        builder.add_next_value(4, 8, 0.5)
        builder.add_next_value(4, 9, 0.5)
        builder.add_next_value(5, 10, 0.5)
        builder.add_next_value(5, 11, 0.5)
        builder.add_next_value(6, 2, 0.5)
        builder.add_next_value(6, 12, 0.5)
        for s in range(7, 13):
            builder.add_next_value(s, s, 1)

        # Build matrix
        transition_matrix = builder.build()

        # State labeling
        state_labeling = stormpy.storage.StateLabeling(13)

        # Add labels
        labels = {"init", "one", "two", "three", "four", "five", "six", "done", "deadlock"}
        for label in labels:
            state_labeling.add_label(label)

        # Set label of state 0
        state_labeling.add_label_to_state("init", 0)

        # Set remaining labels
        state_labeling.add_label_to_state("one", 7)
        state_labeling.add_label_to_state("two", 8)
        state_labeling.add_label_to_state("three", 9)
        state_labeling.add_label_to_state("four", 10)
        state_labeling.add_label_to_state("five", 11)
        state_labeling.add_label_to_state("six", 12)

        # Set label 'done' for multiple states
        state_labeling.set_states("done", stormpy.BitVector(13, [7, 8, 9, 10, 11, 12]))

        # Reward models:
        reward_models = {}
        # Create a vector representing the state-action rewards
        action_reward = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        reward_models["coin_flips"] = stormpy.SparseRewardModel(optional_state_action_reward_vector=action_reward)

        components = stormpy.SparseModelComponents(transition_matrix=transition_matrix, state_labeling=state_labeling, reward_models=reward_models)

        dtmc = stormpy.storage.SparseDtmc(components)

        assert type(dtmc) is stormpy.SparseDtmc
        assert not dtmc.supports_parameters
        assert not dtmc.has_parameters

        # Test transition matrix
        assert dtmc.nr_states == 13
        assert dtmc.nr_transitions == 20
        assert dtmc.transition_matrix.nr_entries == dtmc.nr_transitions
        for e in dtmc.transition_matrix:
            assert e.value() in [0.5, 1]
        for state in dtmc.states:
            assert len(state.actions) <= 1

        # Test state labeling
        assert dtmc.labeling.get_labels() == {"init", "deadlock", "done", "one", "two", "three", "four", "five", "six"}

        # Test reward_models
        assert len(dtmc.reward_models) == 1
        assert not dtmc.reward_models["coin_flips"].has_state_rewards
        assert dtmc.reward_models["coin_flips"].has_state_action_rewards
        for reward in dtmc.reward_models["coin_flips"].state_action_rewards:
            assert reward in [0, 1]
        assert not dtmc.reward_models["coin_flips"].has_transition_rewards
