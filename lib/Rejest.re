open Types;

let expect = Expect.expect;

module Expect_float = Expect.Expect_float;
module Expect_string = Expect.Expect_string;
module Expect_function = Expect_function;

module Expect {
    module Make = Expect.Make;
};

let isRunning = ref(false);

type test = {
  name: string,
  testFunction,
};

let tests: ref(list(test)) = ref([]);

let test = (name: string, testFunction: testFunction) =>
  /* If not running, queue up test */
  if (! isRunning^) {
    let newTest: test = {name, testFunction};
    tests := List.append(tests^, [newTest]);
  } else {
    /* If running, actually run the test */
    TestCollector.collectTest(
      name,
      testFunction,
    );
  };

let run = () => {
  /* For each 'queued' test, call the in-aplty named `collectTest` */
  /* How do we wait for all Lwt promises? */

  let runTests = () => {
    Printexc.record_backtrace(true);
    isRunning := true;
    List.iter(
      t => TestCollector.collectTest(t.name, t.testFunction),
      tests^,
    );
    isRunning := false;

    TestReporter.printSummary();

    TestReporter.passed() ? exit(0) : exit(1);
  };

  runTests();
};
