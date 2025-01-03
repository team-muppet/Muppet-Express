<script>
    import { TabGroup, Tab } from "@skeletonlabs/skeleton";
	let tabSet = $state(0)
</script>

<div class="card p-4">
    Reload test
    tabSet = {tabSet}
    <TabGroup>
        <Tab bind:group={tabSet} name="tab1" value={0}>
            <svelte:fragment slot="lead">(icon)</svelte:fragment>
            <span>(label 1)</span>
        </Tab>
        <Tab bind:group={tabSet} name="tab2" value={1}>(label 2)</Tab>
        <Tab bind:group={tabSet} name="tab3" value={2}>(label 3)</Tab>
        <!-- Tab Panels --->
        <svelte:fragment slot="panel">
            {#if tabSet === 0}
                (tab panel 1 contents) 
            {:else if tabSet === 1}
                (tab panel 2 contents)
            {:else if tabSet === 2}
                (tab panel 3 contents)
            {/if}
        </svelte:fragment>
    </TabGroup>
</div>


			