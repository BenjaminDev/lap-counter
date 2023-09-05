
<script lang="ts">
	
	import { Table } from '@skeletonlabs/skeleton';
	import type { TableSource } from '@skeletonlabs/skeleton';
	import { tableMapperValues } from '@skeletonlabs/skeleton';
	/** @type {import('./$types').PageData} */
    export let data;

	console.log(data);
	console.log(data.params.id);
	const sourceData = [
	{ position: data.id, name: 'BlueCar', lap_count: data.params.id, lap_fastest: 37, lap_slowest: 89, lap_average: 45  },
	{ position: 1, name: 'GreenCar',lap_count: 10, lap_fastest: 37, lap_slowest: 89, lap_average: 45  },
	];
	const tableSimple: TableSource = {
	// A list of heading labels.
	head: ['Name', 'Lap Count', 'Fastest Lap', 'Slowest Lap', 'Average Lap'],
	// The data visibly shown in your table body UI.
	body: tableMapperValues(sourceData, ['name', 'lap_count', 'lap_fastest', 'lap_slowest', 'lap_average']),
	// Optional: The data returned when interactive is enabled and a row is clicked.
	meta: tableMapperValues(sourceData, ['name', 'lap_count', 'lap_fastest', 'lap_slowest', 'lap_average']),
	// Optional: A list of footer labels.
	// foot: ['Total', '', '<code class="code">5</code>']
};
</script>


<div class="container h-full mx-auto flex justify-center items-center">
	<div class="space-y-5 text-3xl"  >

		<h1 class="h1">Car Counter</h1>
		<!-- .table tbody td -->
		<Table source={tableSimple} interactive={true} class="[&>.table tbody td]:text-3xl"/>
	</div>
</div>
